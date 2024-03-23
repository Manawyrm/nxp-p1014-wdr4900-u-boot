#include <common.h>
#include <clock_legacy.h>
#include <linux/delay.h>
#include <console.h>
#include <env.h>
#include <env_internal.h>
#include <init.h>
#include <ns16550.h>
#include <malloc.h>
#include <mmc.h>
#include <nand.h>
#include <i2c.h>
#include <fsl_esdhc.h>
#include <spi_flash.h>
#include <asm/global_data.h>
#include "../../common/spl.h"
#include <mapmem.h>
#include <stdint.h>

static inline uint32_t prbslong(unsigned int *state)
{
	uint32_t data = 0x00;

	for (int bit = 0; bit < 32; ++bit)
	{
		int result = *state & 0x1;
		*state >>= 1;
		if (result)
			*state ^= (1<<14) | (1<<13);

		data |= result << bit;
	}

	return data;
}

static void kitten_memtest_init_prbs(unsigned int *state, ulong seed)
{
	*state = 1;

	for (ulong i = 0; i < seed; ++i)
	{
		prbslong(state);
	}
}

static void kitten_memtest_read_verify(unsigned int *state, ulong seed, vu_long *buf, vu_long *end, ulong start_addr, ulong *errs)
{
	vu_long *addr;
	ulong readback;
	ulong correct_data = 0;

	// Reset PRBS
	kitten_memtest_init_prbs(state, seed);

	for (addr = buf; addr < end; addr++) {
		if (isctrlc()) return;
		schedule();
		if (((ulong)addr & 0xFFFFFul) == 0)
		{
			printf("\rReading 0x%08lx", (ulong)addr);
		}
		readback = *addr;
		correct_data = prbslong(state);
		if (readback != correct_data) {
			ulong offset = addr - buf;

			printf("\nMem error @ 0x%08lx: found %08lx, expected %08lx\n",
			       start_addr + offset * sizeof(vu_long), readback, correct_data);

			*errs += 1;
		}
	}
}

uint32_t kitten_memtest_single(vu_long *buf, uint32_t start_addr, uint32_t end_addr, uint32_t seed, int soak)
{
	unsigned int prbs_state;

	kitten_memtest_init_prbs(&prbs_state, seed);

	volatile uint32_t *end;
	volatile uint32_t *addr;

	uint32_t errs = 0;
	uint32_t length;
	uint32_t readback;
	uint32_t correct_data = 0;

	length = (end_addr - start_addr) / sizeof(uint32_t);
	end = buf + length;

	printf("KittenMemtest PRBS15, writing test pseudo random test sequence\n");

	for (addr = buf; addr < end; addr++)
	{
		if (isctrlc()) return;
		schedule();
		*addr = prbslong(&prbs_state);

		if (((uint32_t)addr & 0xFFFFFul) == 0)
		{
			printf("\rWriting 0x%08lx", (uint32_t)addr);
		}
	}

	if (!soak)
	{
		puts("\nReading...");
		kitten_memtest_read_verify(&prbs_state, seed, buf, end, start_addr, &errs);
	}
	else
	{
		puts("\nLetting data rot for 2sec (auto-refresh test)...\n");
		mdelay(2000);
		kitten_memtest_read_verify(&prbs_state, seed, buf, end, start_addr, &errs);
	}

	printf("\nDone. %lu errors reported.\n", errs);
	return errs;
}

/*
 * Perform a memory test. A more complete alternative test can be
 * configured using CONFIG_SYS_ALT_MEMTEST. The complete test loops until
 * interrupted by ctrl-c or by a failure of one of the sub-tests.
 */
int kitten_memtest(int megabytes, int iteration_limit, int soak)
{
	ulong start, end;
	vu_long scratch_space;
	vu_long *buf, *dummy = &scratch_space;
	ulong count = 0;
	ulong errs = 0;	/* number of errors, or -1 if interrupted */
	ulong pattern = 0;
	int iteration = 0;

	start = 0x00000000;
	end = megabytes * 1024 * 1024;

	if (end < start) {
		printf("Refusing to do empty test\n");
		return -1;
	}

	buf = map_sysmem(start, end - start);
	for (iteration = 0;	!iteration_limit || iteration < iteration_limit; iteration++) {
		if (iteration_limit != 1)
		{
			printf("Iteration: %6d\r\n", iteration + 1);
			debug("\n");
		}

		errs = kitten_memtest_single(buf, start, end, iteration, soak);
		count += errs;
	}

	//for (vu_long *addr = buf; addr < end; addr++)
	//{
	//	*addr = 0x0;
	//}
	unmap_sysmem((void *)buf);

	if (iteration_limit != 1)
	{
		printf("\nTested %d iteration(s) with %lu errors.\n", iteration, count);
	}
	return;
}
