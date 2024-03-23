#include "chainload.h"
#include <common.h>
#include <cpu_func.h>
#include <hang.h>
#include <spi_flash.h>
#include <malloc.h>

ulong spi_offset   = (1 * 1024 * 1024); // 1 MiB from SPI flash start
ulong spi_len      = (1 * 1024 * 1024); // 1 MiB size

ulong load_address = 0x01000000;
ulong exec_address = 0x01036000;

void chainboot_spi()
{
	void (*uboot)(void) __noreturn;
	u32 offset, copy_len = 0;
	unsigned char *buf = NULL;
	struct spi_flash *flash;

	flash = spi_flash_probe(CONFIG_ENV_SPI_BUS, CONFIG_ENV_SPI_CS,
			CONFIG_ENV_SPI_MAX_HZ, CONFIG_ENV_SPI_MODE);
	if (flash == NULL) {
		puts("\nspi_flash_probe failed");
		hang();
	}

	/*
	* Load U-Boot image from SPI flash into DDR
	*/
	spi_flash_read(flash, 0,
		       flash->page_size, (void *)buf);
	offset = spi_offset;

	printf("Loading %lu bytes from SPI offset %08lx into memory at %08lx", spi_len, spi_offset, load_address);
	while (copy_len <= spi_len) {
		spi_flash_read(flash, spi_offset + copy_len, 0x2000,
			       (void *)(load_address
			       + copy_len));
		copy_len = copy_len + 0x2000;
		putc('.');
	}

	printf("\nDone. Flushing cache.\n");

	/*
	* Jump to U-Boot image
	*/
	flush_cache(load_address, spi_len);

	printf("Jumping to %08lx now!\n", exec_address);
	uboot = (void *)exec_address;
	(*uboot)();

}