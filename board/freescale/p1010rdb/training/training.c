#include <stdio.h>
#include <string.h>
#include <serial.h>
#include <vsprintf.h>
#include <stdint.h>

#include <common.h>
#include <linux/delay.h>
#include <console.h>
#include <init.h>
#include <malloc.h>
#include <asm/global_data.h>
#include <mapmem.h>

#include "training.h"
#include "microrl.h"
#include "memtest.h"
#include "registers.h"
#include "chainload.h"

#define _MMIO_BYTE(mem_addr) (*(volatile uint8_t *)(mem_addr))
#define _MMIO_LONG(mem_addr) (*(volatile uint32_t *)(mem_addr))

#define _CMD_HELP  "help"
#define _CMD_CLEAR "clear"
#define _CMD_TEST  "test"
#define _CMD_SOAK  "soak"
#define _CMD_REGS  "regs"
#define _CMD_SET   "set"
#define _CMD_PEEK  "peek"
#define _CMD_POKE  "poke"
#define _CMD_BOOT  "boot"
#define _CMD_ADDRFILL  "addrfill"

#define _NUM_OF_CMD 10

char * keyworld [] = {_CMD_HELP, _CMD_CLEAR, _CMD_TEST, _CMD_SOAK, _CMD_REGS, _CMD_SET, _CMD_PEEK, _CMD_POKE, _CMD_BOOT, _CMD_ADDRFILL};
char * compl_world [_NUM_OF_CMD + 1];

int isctrlc(void)
{
	if (tstc())
	{
		switch (getchar())
		{
			case 0x03:		/* ^C - Control C */
				return 1;
			default:
				break;
		}
	}

	return 0;
}

int execute (int argc, const char * const * argv)
{
	if (strcmp (argv[0], _CMD_HELP) == 0 || strcmp (argv[0], "?") == 0) {
		serial_puts ("WDR4900 memory training shell\n\r");
		serial_puts ("\n\r");
		serial_puts ("Commands:\n\r");
		serial_puts ("\thelp                               Display this message\n\r");
		serial_puts ("\tclear                              Wipe screen\n\r");
		serial_puts ("\ttest [megabytes] [iterations]      Run memtest\n\r");
		serial_puts ("\tsoak [megabytes] [iterations]      Run memtest with soak delay\n\r");
		serial_puts ("\tregs                               Dump DDR registers\n\r");
		serial_puts ("\tset  <register> <value>            Change 32bit reg (read, write, read)\n\r");
		serial_puts ("\tpeek <register>                    Get 32bit reg\n\r");
		serial_puts ("\tpoke <register> <value>            Set 32bit reg (write only)\n\r");
		serial_puts ("\tboot                               Load TP-Link vendor u-boot\n\r");

	} else if (strcmp (argv[0], _CMD_CLEAR) == 0) {
		serial_puts ("\033[2J");    // ESC seq for clear entire screen
		serial_puts ("\033[H");     // ESC seq for move cursor at left-top corner
	} else if (strcmp (argv[0], _CMD_TEST) == 0 || strcmp (argv[0], _CMD_SOAK) == 0) {
		int megabytes = 128;
		int iterations = 1;

		if (argc == 2)
		{
			sscanf(argv[1], "%d", &megabytes);
		}
		if (argc == 3)
		{
			sscanf(argv[1], "%d", &megabytes);
			sscanf(argv[2], "%d", &iterations);
		}

		kitten_memtest(megabytes, iterations, strcmp (argv[0], _CMD_SOAK) == 0);
	} else if (strcmp (argv[0], _CMD_REGS) == 0) {
		registers_dump();
	} else if (strcmp (argv[0], _CMD_SET) == 0) {
		if (argc != 3)
		{
			serial_puts ("Usage:\n\r");
			serial_puts ("\tset <register> <value>\n\r");
		}
		else
		{
			uint32_t reg = 0;
			uint32_t value = 0;
			sscanf(argv[1], "%x", &reg);
			sscanf(argv[2], "%x", &value);

			printf("Changing %08x from %08x to %08x...\n", reg, _MMIO_LONG(reg), value);
			_MMIO_LONG(reg) = value;
			printf("         %08x =    %08x\n", reg, _MMIO_LONG(reg));
		}
	} else if (strcmp (argv[0], _CMD_PEEK) == 0) {
		if (argc != 2)
		{
			serial_puts ("Usage:\n\r");
			serial_puts ("\tpeek <register>\n\r");
		}
		else
		{
			uint32_t reg = 0;
			sscanf(argv[1], "%x", &reg);
			printf("%08x = %08x\n", reg, _MMIO_LONG(reg));
		}
	} else if (strcmp (argv[0], _CMD_POKE) == 0) {
		if (argc != 3)
		{
			serial_puts ("Usage:\n\r");
			serial_puts ("\tpoke <register> <value>\n\r");
		}
		else
		{
			uint32_t reg = 0;
			uint32_t value = 0;
			sscanf(argv[1], "%x", &reg);
			sscanf(argv[2], "%x", &value);

			printf("Changing %08x to %08x...\n", reg, value);
			_MMIO_LONG(reg) = value;
		}
	} else if (strcmp (argv[0], _CMD_ADDRFILL) == 0) {
		if (argc != 3)
		{
			serial_puts ("Usage:\n\r");
			serial_puts ("\taddrfill <start> <end>\n\r");
		}
		else
		{
			uint32_t start_addr = 0;
			uint32_t end_addr = 0;
			sscanf(argv[1], "%x", &start_addr);
			sscanf(argv[2], "%x", &end_addr);

			printf("Filling %08x to %08x...\n", start_addr, end_addr);

			volatile uint32_t *addr = (volatile uint32_t *) start_addr;

			for (; (uint32_t) addr < end_addr; addr++)
			{
				*addr = (uint32_t) addr;
			}
		}
	} else if (strcmp (argv[0], _CMD_BOOT) == 0) {
		chainboot_spi();
	} else {
		serial_puts ("command: '");
		serial_puts ((char*)argv[0]);
		serial_puts ("' not found.\n\r");
	}
		
	return 0;
}

char ** complet (int argc, const char * const * argv)
{
	int j = 0;

	compl_world [0] = NULL;

	// if there is token in cmdline
	if (argc == 1) {
		// get last entered token
		char * bit = (char*)argv [argc-1];
		// iterate through our available token and match it
		for (int i = 0; i < _NUM_OF_CMD; i++) {
			// if token is matched (text is part of our token starting from 0 char)
			if (strstr(keyworld [i], bit) == keyworld [i]) {
				// add it to completion set
				compl_world [j++] = keyworld [i];
			}
		}
	} else { // if there is no token in cmdline, just print all available token
		for (; j < _NUM_OF_CMD; j++) {
			compl_world[j] = keyworld [j];
		}
	}

	// note! last ptr in array always must be NULL!!!
	compl_world [j] = NULL;
	// return set of variants
	return compl_world;
}

void training_user_interface()
{
	serial_puts ("training_user_interface()\n\r");
	microrl_t rl;
	microrl_t *prl = &rl;
	// call init with ptr to microrl instance and print callback
	microrl_init (prl, serial_puts);
	// set callback for execute
	microrl_set_execute_callback (prl, execute);
	// set callback for completion (optionally)
	microrl_set_complete_callback (prl, complet);
	// set callback for ctrl+c handling (optionally)
	//microrl_set_sigint_callback (prl, sigint);

	while (1)
	{
		if (serial_tstc())
		{
			microrl_insert_char (prl, serial_getc());
		}
		schedule();
	}
}