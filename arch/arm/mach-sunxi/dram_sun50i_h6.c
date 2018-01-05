/*
 * sun50i platform dram controller init
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/dram.h>
#include <linux/kconfig.h>

/* Some callback functions, needed for the libdram blob */

void __usdelay(unsigned long us)
{
	udelay(us);
}

int set_ddr_voltage(int set_vol)
{
	/* DCDC5 is already set as 1.5V by default */
	return 0;
}

int axp_set_aldo3(int param1, int param2)
{
	/* Only needed for DDR4 */
	return 0;
}
/* A piece of data */
struct dram_para
{
	unsigned int dram_clk;
	unsigned int dram_type;
	unsigned int dram_zq;
	unsigned int dram_odt_en;
	unsigned int dram_para1;
	unsigned int dram_para2;
	unsigned int dram_mr0;
	unsigned int dram_mr1;
	unsigned int dram_mr2;
	unsigned int dram_mr3;
	unsigned int dram_mr4;
	unsigned int dram_mr5;
	unsigned int dram_mr6;
	unsigned int dram_tpr0;
	unsigned int dram_tpr1;
	unsigned int dram_tpr2;
	unsigned int dram_tpr3;
	unsigned int dram_tpr4;
	unsigned int dram_tpr5;
	unsigned int dram_tpr6;
	unsigned int dram_tpr7;
	unsigned int dram_tpr8;
	unsigned int dram_tpr9;
	unsigned int dram_tpr10;
	unsigned int dram_tpr11;
	unsigned int dram_tpr12;
	unsigned int dram_tpr13;
	unsigned int dram_tpr14;
	unsigned int dram_tpr15;
	unsigned int dram_tpr16;
	unsigned int dram_tpr17;
	unsigned int dram_tpr18;
};

struct dram_para dram_para = {
	.dram_clk     = 0,
	.dram_type    = 7,
	.dram_zq      = 0x3b3bfb,
	.dram_odt_en  = 0x31,
	.dram_para1   = 0x30FA,
	.dram_para2   = 0x04000000,
	.dram_mr0     = 0x1c70,
	.dram_mr1     = 0x40,
	.dram_mr2     = 0x18,
	.dram_mr3     = 0x1,
	.dram_mr4     = 0x0,
	.dram_mr5     = 0x400,
	.dram_mr6     = 0x848,
	.dram_tpr0    = 0x0048A192,
	.dram_tpr1    = 0x01b1a94b,
	.dram_tpr2    = 0x00061043,
	.dram_tpr3    = 0x78787896,
	.dram_tpr4    = 0x0000,
	.dram_tpr5    = 0x0,
	.dram_tpr6    = 0x09090900,
	.dram_tpr7    = 0x4d462a3e,
	.dram_tpr8    = 0x0,
	.dram_tpr9    = 0,
	.dram_tpr10   = 0x0,
	.dram_tpr11   = 0x00440000,
	.dram_tpr12   = 0x0,
	.dram_tpr13   = 0x0000,
};
/* Entry point to the libdram blob */
unsigned long init_DRAM(u32, struct dram_para *);

unsigned long sunxi_dram_init(void)
{
	init_DRAM(0, &dram_para);
	return get_ram_size((long *)PHYS_SDRAM_0, PHYS_SDRAM_0_SIZE);
}
