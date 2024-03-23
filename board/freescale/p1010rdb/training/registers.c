#include "registers.h"
#include "training.h"
#include <stdio.h>
#include <string.h>

#define _MMIO_BYTE(mem_addr) (*(volatile unsigned char *)(mem_addr))
#define _MMIO_LONG(mem_addr) (*(volatile unsigned long *)(mem_addr))

void registers_dump()
{
	printf   ("FF702000 DDR_CS0_BNDS = %08lx\n", 				_MMIO_LONG(0xFF702000));
	printf   ("FF702008 DDR_CS1_BNDS = %08lx\n", 				_MMIO_LONG(0xFF702008));
	printf   ("FF702010 DDR_CS2_BNDS = %08lx\n", 				_MMIO_LONG(0xFF702010));
	printf   ("FF702018 DDR_CS3_BNDS = %08lx\n", 				_MMIO_LONG(0xFF702018));
	printf   ("FF702080 DDR_CS0_CONFIG = %08lx\n", 				_MMIO_LONG(0xFF702080));
	printf   ("FF702084 DDR_CS1_CONFIG = %08lx\n", 				_MMIO_LONG(0xFF702084));
	printf   ("FF702088 DDR_CS2_CONFIG = %08lx\n", 				_MMIO_LONG(0xFF702088));
	printf   ("FF70208C DDR_CS3_CONFIG = %08lx\n", 				_MMIO_LONG(0xFF70208C));
	printf   ("FF7020C0 DDR_CS0_CONFIG_2 = %08lx\n", 			_MMIO_LONG(0xFF7020C0));
	printf   ("FF7020C4 DDR_CS1_CONFIG_2 = %08lx\n", 			_MMIO_LONG(0xFF7020C4));
	printf   ("FF7020C8 DDR_CS2_CONFIG_2 = %08lx\n", 			_MMIO_LONG(0xFF7020C8));
	printf   ("FF7020CC DDR_CS3_CONFIG_2 = %08lx\n", 			_MMIO_LONG(0xFF7020CC));
	printf   ("FF702100 DDR_TIMING_CFG_3 = %08lx\n", 			_MMIO_LONG(0xFF702100));
	printf   ("FF702104 DDR_TIMING_CFG_0 = %08lx\n", 			_MMIO_LONG(0xFF702104));
	printf   ("FF702108 DDR_TIMING_CFG_1 = %08lx\n", 			_MMIO_LONG(0xFF702108));
	printf   ("FF70210C DDR_TIMING_CFG_2 = %08lx\n", 			_MMIO_LONG(0xFF70210C));
	printf   ("FF702110 DDR_DDR_SDRAM_CFG = %08lx\n", 			_MMIO_LONG(0xFF702110));
	printf   ("FF702114 DDR_DDR_SDRAM_CFG_2 = %08lx\n", 		_MMIO_LONG(0xFF702114));
	printf   ("FF702118 DDR_DDR_SDRAM_MODE = %08lx\n", 			_MMIO_LONG(0xFF702118));
	printf   ("FF70211C DDR_DDR_SDRAM_MODE_2 = %08lx\n", 		_MMIO_LONG(0xFF70211C));
	printf   ("FF702120 DDR_DDR_SDRAM_MD_CNTL = %08lx\n", 		_MMIO_LONG(0xFF702120));
	printf   ("FF702124 DDR_DDR_SDRAM_INTERVAL = %08lx\n", 		_MMIO_LONG(0xFF702124));
	printf   ("FF702128 DDR_DDR_DATA_INIT = %08lx\n", 			_MMIO_LONG(0xFF702128));
	printf   ("FF702130 DDR_DDR_SDRAM_CLK_CNTL = %08lx\n", 		_MMIO_LONG(0xFF702130));
	printf   ("FF702148 DDR_DDR_INIT_ADDR = %08lx\n", 			_MMIO_LONG(0xFF702148));
	printf   ("FF70214C DDR_DDR_INIT_EXT_ADDR = %08lx\n", 		_MMIO_LONG(0xFF70214C));
	printf   ("FF702160 DDR_TIMING_CFG_4 = %08lx\n", 			_MMIO_LONG(0xFF702160));
	printf   ("FF702164 DDR_TIMING_CFG_5 = %08lx\n", 			_MMIO_LONG(0xFF702164));
	printf   ("FF702170 DDR_DDR_ZQ_CNTL = %08lx\n", 			_MMIO_LONG(0xFF702170));
	printf   ("FF702174 DDR_DDR_WRLVL_CNTL = %08lx\n", 			_MMIO_LONG(0xFF702174));
	printf   ("FF70217C DDR_DDR_SR_CNTR = %08lx\n", 			_MMIO_LONG(0xFF70217C));
	printf   ("FF702180 DDR_DDR_SDRAM_RCW_1 = %08lx\n", 		_MMIO_LONG(0xFF702180));
	printf   ("FF702184 DDR_DDR_SDRAM_RCW_2 = %08lx\n", 		_MMIO_LONG(0xFF702184));
	printf   ("FF702190 DDR_DDR_WRLVL_CNTL_2 = %08lx\n", 		_MMIO_LONG(0xFF702190));
	printf   ("FF702194 DDR_DDR_WRLVL_CNTL_3 = %08lx\n", 		_MMIO_LONG(0xFF702194));
	printf   ("FF702200 DDR_DDR_SDRAM_MODE_3 = %08lx\n", 		_MMIO_LONG(0xFF702200));
	printf   ("FF702204 DDR_DDR_SDRAM_MODE_4 = %08lx\n", 		_MMIO_LONG(0xFF702204));
	printf   ("FF702208 DDR_DDR_SDRAM_MODE_5 = %08lx\n", 		_MMIO_LONG(0xFF702208));
	printf   ("FF70220C DDR_DDR_SDRAM_MODE_6 = %08lx\n", 		_MMIO_LONG(0xFF70220C));
	printf   ("FF702210 DDR_DDR_SDRAM_MODE_7 = %08lx\n", 		_MMIO_LONG(0xFF702210));
	printf   ("FF702214 DDR_DDR_SDRAM_MODE_8 = %08lx\n", 		_MMIO_LONG(0xFF702214));
	printf   ("FF702B20 DDR_DDRDSR_1 = %08lx\n", 				_MMIO_LONG(0xFF702B20));
	printf   ("FF702B24 DDR_DDRDSR_2 = %08lx\n", 				_MMIO_LONG(0xFF702B24));
	printf   ("FF702B28 DDR_DDRCDR_1 = %08lx\n", 				_MMIO_LONG(0xFF702B28));
	printf   ("FF702B2C DDR_DDRCDR_2 = %08lx\n", 				_MMIO_LONG(0xFF702B2C));
	printf   ("FF702BF8 DDR_DDR_IP_REV1 = %08lx\n", 			_MMIO_LONG(0xFF702BF8));
	printf   ("FF702BFC DDR_DDR_IP_REV2 = %08lx\n", 			_MMIO_LONG(0xFF702BFC));
}