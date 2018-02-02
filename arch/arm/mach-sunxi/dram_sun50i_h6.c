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
	unsigned int clk;
	unsigned int ranks;
	unsigned int type;
};

/*struct dram_para dram_para = {
	.dram_clk     = 0,
	.dram_type    = SUNXI_DRAM_TYPE_LPDDR3,
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
};*/

struct dram_para dram_para = {
	.dram_clk     = 744,
	.dram_type    = SUNXI_DRAM_TYPE_LPDDR3,
	.dram_zq      = 0x3b3bfb,
	.dram_odt_en  = 0x31,
	.dram_para1   = 0x30eb,
	.dram_para2   = 0x1000,
	.dram_mr0     = 0x0,
	.dram_mr1     = 0x43,
	.dram_mr2     = 0x1a,
	.dram_mr3     = 0x1,
	.dram_mr4     = 0x0,
	.dram_mr5     = 0x400,
	.dram_mr6     = 0x848,
	.dram_tpr0    = 0x1249a25b,
	.dram_tpr1    = 0x11b1b1d0,
	.dram_tpr2    = 0x84f02d,
	.dram_tpr3    = 0x78787896,
	.dram_tpr4    = 0x0000,
	.dram_tpr5    = 0x0,
	.dram_tpr6    = 0x09090900,
	.dram_tpr7    = 0x4d462a3e,
	.dram_tpr8    = 0x0,
	.dram_tpr9    = 0,
	.dram_tpr10   = 0xa0,
	.dram_tpr11   = 0x200000,
	.dram_tpr12   = 0x2222,
	.dram_tpr13   = 0x6003,
	.clk     = 744,
	.type    = SUNXI_DRAM_TYPE_LPDDR3,
	.ranks   = 2
};

enum {
	MBUS_PORT_CPU           = 0,
	MBUS_PORT_GPU           = 1,
	MBUS_PORT_MAHB          = 2,
	MBUS_PORT_DMA           = 3,
	MBUS_PORT_VE            = 4,
	MBUS_PORT_CE            = 5,
	MBUS_PORT_TSC0          = 6,
	MBUS_PORT_NDFC0         = 8,
	MBUS_PORT_CSI0          = 11,
	MBUS_PORT_DI0           = 14,
	MBUS_PORT_DI1           = 15,
	MBUS_PORT_DE300         = 16,
	MBUS_PORT_IOMMU         = 25,
	MBUS_PORT_VE2           = 26,
	MBUS_PORT_USB3        = 37,
	MBUS_PORT_PCIE          = 38,
	MBUS_PORT_VP9           = 39,
	MBUS_PORT_HDCP2       = 40,
};

enum {
	MBUS_QOS_LOWEST = 0,
	MBUS_QOS_LOW,
	MBUS_QOS_HIGH,
	MBUS_QOS_HIGHEST
};

inline void mbus_configure_port(u8 port,
				bool bwlimit,
				bool priority,
				u8 qos,         /* MBUS_QOS_LOWEST .. MBUS_QOS_HIGEST */
				u8 waittime,    /* 0 .. 0xf */
				u8 acs,         /* 0 .. 0xff */
				u16 bwl0,       /* 0 .. 0xffff, bandwidth limit in MB/s */
				u16 bwl1,
				u16 bwl2)
{
	struct sunxi_mctl_com_reg * const mctl_com =
			(struct sunxi_mctl_com_reg *)SUNXI_DRAM_COM_BASE;

	const u32 cfg0 = ( (bwlimit ? (1 << 0) : 0)
			   | (priority ? (1 << 1) : 0)
			   | ((qos & 0x3) << 2)
			   | ((waittime & 0xf) << 4)
			   | ((acs & 0xff) << 8)
			   | (bwl0 << 16) );
	const u32 cfg1 = ((u32)bwl2 << 16) | (bwl1 & 0xffff);

	debug("MBUS port %d cfg0 %08x cfg1 %08x\n", port, cfg0, cfg1);
	writel(cfg0, &mctl_com->master[port].cfg0);
	writel(cfg1, &mctl_com->master[port].cfg1);
}

#define MBUS_CONF(port, bwlimit, qos, acs, bwl0, bwl1, bwl2)	\
	mbus_configure_port(MBUS_PORT_ ## port, bwlimit, false, \
			    MBUS_QOS_ ## qos, 0, acs, bwl0, bwl1, bwl2)

static void mctl_set_master_priority(void)
{
	struct sunxi_mctl_com_reg * const mctl_com =
			(struct sunxi_mctl_com_reg *)SUNXI_DRAM_COM_BASE;

	/* enable bandwidth limit windows and set windows size 1us */
	writel(399, &mctl_com->tmr);
	writel(BIT(16), &mctl_com->bwcr);

	MBUS_CONF(  CPU,  true, HIGHEST, 0,  256,  128,  100);
	MBUS_CONF(  GPU,  true,    HIGH, 0, 1536, 1400,  256);
	MBUS_CONF( MAHB,  true, HIGHEST, 0,  512,  256,   96);
	MBUS_CONF(  DMA,  true,    HIGH, 0,  256,  100,   80);
	MBUS_CONF(   VE,  true,    HIGH, 2, 8192, 5500, 5000);
	MBUS_CONF(   CE,  true,    HIGH, 2,  100,   64,   32);
	MBUS_CONF( TSC0,  true,    HIGH, 2,  100,   64,   32);
	MBUS_CONF(NDFC0,  true,    HIGH, 0,  256,  128,   64);
	MBUS_CONF( CSI0,  true,    HIGH, 0,  256,  128,  100);
	MBUS_CONF(  DI0,  true,    HIGH, 0, 1024,  256,   64);
	MBUS_CONF(DE300,  true, HIGHEST, 6, 8192, 2800, 2400);
	MBUS_CONF(IOMMU,  true, HIGHEST, 0,  100,   64,   32);
	MBUS_CONF(  VE2,  true,    HIGH, 2, 8192, 5500, 5000);
	MBUS_CONF( USB3,  true,    HIGH, 0,  256,  128,   64);
	MBUS_CONF( PCIE,  true,    HIGH, 2,  100,   64,   32);
	MBUS_CONF(  VP9,  true,    HIGH, 2, 8192, 5500, 5000);
	MBUS_CONF(HDCP2,  true,    HIGH, 2,  100,   64,   32);
}

static u32 mr_lpddr3[12] = {
	0x00000000, 0x00000043, 0x0000001a, 0x00000001,
	0x00000000, 0x00000000, 0x00000048, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000003,
};

static u32 dramtmg_lpddr3[15] = {
	0x0e130c10, 0x0003031b, 0x03060809, 0x0050500c,
	0x09020407, 0x05050503, 0x02020005, 0x00000202,
	0x04040404, 0x00020208, 0x001c180a, 0x440c021c,
	0x00000010, 0x00000010, 0x00000052,
};

static u32 dtpr_lpddr3[7] = {
	0x08200e06, 0x2826040a, 0x000600ff, 0x00ff0502,
	0x009e0806, 0x00361206, 0x00000505,
};


static void mctl_sys_init(struct dram_para *para)
{
	struct sunxi_ccm_reg * const ccm =
			(struct sunxi_ccm_reg *)SUNXI_CCM_BASE;
	struct sunxi_mctl_com_reg * const mctl_com =
			(struct sunxi_mctl_com_reg *)SUNXI_DRAM_COM_BASE;
	struct sunxi_mctl_ctl_reg * const mctl_ctl =
			(struct sunxi_mctl_ctl_reg *)SUNXI_DRAM_CTL0_BASE;

	/* Put all DRAM-related blocks to reset state */
	clrbits_le32(&ccm->mbus_cfg, MBUS_ENABLE | MBUS_RESET);
	writel(0, &ccm->dram_gate_reset);
	clrbits_le32(&ccm->pll5_cfg, CCM_PLL5_CTRL_EN);
	clrbits_le32(&ccm->dram_clk_cfg, DRAM_MOD_RESET);

	udelay(5);

	/* Set PLL5 rate to doubled DRAM clock rate */
	writel(CCM_PLL5_CTRL_EN | CCM_PLL5_LOCK_EN |
	       CCM_PLL5_CTRL_N(para->dram_clk / 24 - 1), &ccm->pll5_cfg);
	mctl_await_completion(&ccm->pll5_cfg, CCM_PLL5_LOCK, CCM_PLL5_LOCK);

	/* Configure DRAM mod clock */
	writel(DRAM_CLK_SRC_PLL5, &ccm->dram_clk_cfg);
	setbits_le32(&ccm->dram_clk_cfg, DRAM_CLK_UPDATE);
	writel(BIT(0) | BIT(RESET_SHIFT), &ccm->dram_gate_reset);

	/* Disable all channels */
	writel(0, &mctl_com->maer0);
	writel(0, &mctl_com->maer1);
	writel(0, &mctl_com->maer2);

	/* Configure MBUS and enable DRAM mod reset */
	setbits_le32(&ccm->mbus_cfg, MBUS_RESET);
	setbits_le32(&ccm->mbus_cfg, MBUS_ENABLE);
	setbits_le32(&ccm->dram_clk_cfg, DRAM_MOD_RESET);
	udelay(5);

	/* Unknown hack from the BSP, which enables access of mctl_ctl regs */
	writel(0x8000, &mctl_ctl->unk_0x00c);
}

static const u32 addrmap_default[12] = {
	0x00000016, 0x00090909, 0x00000000, 0x00000000,
	0x00001f00, 0x08080808, 0x0f0f0808, 0x00000f0f,
	0x00003f3f, 0x00000000, 0x00000000, 0x00000000,
};

static void mctl_set_addrmap(struct dram_para *para)
{
	struct sunxi_mctl_ctl_reg * const mctl_ctl =
			(struct sunxi_mctl_ctl_reg *)SUNXI_DRAM_CTL0_BASE;

	memcpy(&mctl_ctl->addrmap, addrmap_default, sizeof(addrmap_default));
}

static void mctl_com_init(struct dram_para *para)
{
	struct sunxi_mctl_com_reg * const mctl_com =
			(struct sunxi_mctl_com_reg *)SUNXI_DRAM_COM_BASE;
	struct sunxi_mctl_ctl_reg * const mctl_ctl =
			(struct sunxi_mctl_ctl_reg *)SUNXI_DRAM_CTL0_BASE;
	struct sunxi_mctl_phy_reg * const mctl_phy =
			(struct sunxi_mctl_phy_reg *)SUNXI_DRAM_PHY0_BASE;
	u32 unk_008_update_val;

	mctl_set_addrmap(para);

	setbits_le32(&mctl_com->cr, BIT(31));
	/*
	 * This address is magic; it's in SID memory area, but there's no
	 * known definition of it.
	 * On my Pine H64 board it has content 7.
	 */
	if (readl(0x03006100) == 7)
		clrbits_le32(&mctl_com->cr, BIT(27));
	else if (readl(0x03006100) == 3)
		setbits_le32(&mctl_com->cr, BIT(27));

	if (para->dram_clk > 408)
		unk_008_update_val = 0xf00;
	else if (para->dram_clk > 246)
		unk_008_update_val = 0x1f00;
	else
		unk_008_update_val = 0x3f00;
	clrsetbits_le32(&mctl_com->unk_0x008, 0x3f00, unk_008_update_val);

	int ranks = 2;

	/* TODO: half DQ, non-LPDDR3 types */
	writel(MSTR_DEVICETYPE_LPDDR3 | MSTR_BUSWIDTH_FULL |
	       MSTR_BURST_LENGTH(8) | MSTR_ACTIVE_RANKS(ranks) |
	       0x80000000, &mctl_ctl->mstr);
	writel(DCR_LPDDR3 | DCR_DDR8BANK | 0x400, &mctl_phy->dcr);

	if (ranks == 2)
		writel(0x0303, &mctl_ctl->odtmap);
	else
		writel(0x0201, &mctl_ctl->odtmap);

	/* TODO: calculate the value, non-LPDDR3 types */
	writel(0x09020400, &mctl_ctl->odtcfg);

	/* TODO: half DQ */
}

static void mctl_phy_pir_init(u32 val)
{
	struct sunxi_mctl_phy_reg * const mctl_phy =
			(struct sunxi_mctl_phy_reg *)SUNXI_DRAM_PHY0_BASE;

	writel(val | BIT(0), &mctl_phy->pir);
	mctl_await_completion(&mctl_phy->pgsr[1], BIT(0), BIT(0));
}

static void mctl_bit_delay_set(struct dram_para *para)
{
	/* TODO */
	struct sunxi_mctl_phy_reg * const mctl_phy =
	(struct sunxi_mctl_phy_reg *)SUNXI_DRAM_PHY0_BASE;
	int i;
	u32 val;

	writel(0x07080708, &mctl_phy->dx[0].bdlr0);
	writel(0x0907070b, &mctl_phy->dx[0].bdlr1);
	writel(0x00000001, &mctl_phy->dx[0].bdlr2);
	writel(0x07000505, &mctl_phy->dx[1].bdlr0);
	writel(0x03040302, &mctl_phy->dx[1].bdlr1);
	writel(0x00040402, &mctl_phy->dx[1].bdlr2);
	writel(0x06050004, &mctl_phy->dx[2].bdlr0);
	writel(0x07060607, &mctl_phy->dx[2].bdlr1);
	writel(0x00000003, &mctl_phy->dx[2].bdlr2);
	writel(0x05020402, &mctl_phy->dx[3].bdlr0);
	writel(0x06060705, &mctl_phy->dx[3].bdlr1);
	writel(0x00000002, &mctl_phy->dx[3].bdlr2);
	clrbits_le32(&mctl_phy->pgcr[0], BIT(26));

	writel(0x0c060406, &mctl_phy->dx[0].bdlr3);
	writel(0x0e0a0a10, &mctl_phy->dx[0].bdlr4);
	writel(0x00000004, &mctl_phy->dx[0].bdlr5);
	writel(0x00040400, &mctl_phy->dx[0].bdlr6);
	writel(0x0b060809, &mctl_phy->dx[1].bdlr3);
	writel(0x05040807, &mctl_phy->dx[1].bdlr4);
	writel(0x00000004, &mctl_phy->dx[1].bdlr5);
	writel(0x00040400, &mctl_phy->dx[1].bdlr6);
	writel(0x08060407, &mctl_phy->dx[2].bdlr3);
	writel(0x080a0609, &mctl_phy->dx[2].bdlr4);
	writel(0x00000004, &mctl_phy->dx[2].bdlr5);
	writel(0x00040400, &mctl_phy->dx[2].bdlr6);
	writel(0x06090409, &mctl_phy->dx[3].bdlr3);
	writel(0x0f0e0b0a, &mctl_phy->dx[3].bdlr4);
	writel(0x00000004, &mctl_phy->dx[3].bdlr5);
	writel(0x00040400, &mctl_phy->dx[3].bdlr6);
	setbits_le32(&mctl_phy->pgcr[0], BIT(26));

	/* Maybe 0x0a0a0a0a? */
	for (i = 1; i < 14; i++) {
		val = readl(&mctl_phy->acbdlr[i]);
		val += 0x0a0a0a0a;
		writel(val, &mctl_phy->acbdlr[i]);
	}
}

static void mctl_channel_init(int x, struct dram_para *para)
{
	struct sunxi_mctl_com_reg * const mctl_com =
			(struct sunxi_mctl_com_reg *)SUNXI_DRAM_COM_BASE;
	struct sunxi_mctl_ctl_reg * const mctl_ctl =
			(struct sunxi_mctl_ctl_reg *)SUNXI_DRAM_CTL0_BASE;
	struct sunxi_mctl_phy_reg * const mctl_phy =
			(struct sunxi_mctl_phy_reg *)SUNXI_DRAM_PHY0_BASE;
	int i;
	u32 val;

	setbits_le32(&mctl_ctl->dfiupd[0], BIT(31) | BIT(30));
	setbits_le32(&mctl_ctl->zqctl[0], BIT(31) | BIT(30));
	writel(0x2f05, &mctl_ctl->sched[0]);
	setbits_le32(&mctl_ctl->rfshctl3, BIT(0));
	setbits_le32(&mctl_ctl->dfimisc, BIT(0));
	setbits_le32(&mctl_ctl->unk_0x00c, BIT(8));
	clrsetbits_le32(&mctl_phy->pgcr[1], 0x180, 0xc0);
	/* TODO: non-LPDDR3 types */
	clrsetbits_le32(&mctl_phy->pgcr[2], GENMASK(17, 0), ns_to_t(7800));
	clrbits_le32(&mctl_phy->pgcr[6], BIT(0));
	clrsetbits_le32(&mctl_phy->dxccr, 0xee0, 0x220);
	/* TODO: VT compensation */
	clrsetbits_le32(&mctl_phy->dsgcr, BIT(0), 0x440060);
	clrbits_le32(&mctl_phy->vtcr[1], BIT(1));

	for (i = 0; i < 4; i++)
		clrsetbits_le32(&mctl_phy->dx[i].gcr[0], 0xe00, 0x800);
	for (i = 0; i < 4; i++)
		clrsetbits_le32(&mctl_phy->dx[i].gcr[2], 0xffff, 0x5555);
	for (i = 0; i < 4; i++)
		clrsetbits_le32(&mctl_phy->dx[i].gcr[3], 0x3030, 0x1010);

	udelay(100);

	if (para->ranks == 2)
		clrsetbits_le32(&mctl_phy->dtcr[1], 2, 0x30000);
	else
		clrsetbits_le32(&mctl_phy->dtcr[1], 0x30000, 0x10000);

	writel(0, &mctl_phy->rankidr);
	writel(0x10000, &mctl_phy->odtcr);
	if (para->ranks == 2) {
		writel(0x00010001, &mctl_phy->rankidr);
		writel(0x20000, &mctl_phy->odtcr);
	} else {
		/* ? */
		writel(0x0, &mctl_phy->rankidr);
		writel(0x10000, &mctl_phy->odtcr);
	}

	/* TODO: non-LPDDR3 types */
	clrsetbits_le32(&mctl_phy->dtcr[0], 0xFFFFFFF, 0x10000040);
	if (para->clk <= 792) {
		if (para->clk <= 672) {
			if (para->clk <= 600)
				val = 0x300;
			else
				val = 0x400;
		} else {
			val = 0x500;
		}
	} else {
		val = 0x600;
	}
	/* FIXME: NOT REVIEWED YET */
	clrsetbits_le32(&mctl_phy->zq[0].zqcr, 0x700, val);
	clrsetbits_le32(&mctl_phy->zq[0].zqpr[0], 0xff,
			CONFIG_DRAM_ZQ & 0xff);
	clrbits_le32(&mctl_phy->zq[0].zqor[0], 0xfffff);
	setbits_le32(&mctl_phy->zq[0].zqor[0], (CONFIG_DRAM_ZQ >> 8) & 0xff);
	setbits_le32(&mctl_phy->zq[0].zqor[0], (CONFIG_DRAM_ZQ & 0xf00) - 0x100);
	setbits_le32(&mctl_phy->zq[0].zqor[0], (CONFIG_DRAM_ZQ & 0xff00) << 4);
	clrbits_le32(&mctl_phy->zq[1].zqpr[0], 0xfffff);
	setbits_le32(&mctl_phy->zq[1].zqpr[0], (CONFIG_DRAM_ZQ >> 16) & 0xff);
	setbits_le32(&mctl_phy->zq[1].zqpr[0], ((CONFIG_DRAM_ZQ >> 8) & 0xf00) - 0x100);
	setbits_le32(&mctl_phy->zq[1].zqpr[0], (CONFIG_DRAM_ZQ & 0xff0000) >> 4);
	if (para->type == SUNXI_DRAM_TYPE_LPDDR3) {
		for (i = 1; i <= 14; i++)
			writel(0x06060606, &mctl_phy->acbdlr[i]);
	}
	/* TODO: non-LPDDR3 types */
	mctl_phy_pir_init(0xf562);

	/* TODO: non-LPDDR3 types */
	for (i = 0; i < 4; i++)
		writel(0x00000909, &mctl_phy->dx[i].gcr[5]);

	for (i = 0; i < 4; i++) {
		if (IS_ENABLED(CONFIG_DRAM_ODT_EN))
			val = 0x0;
		else
			val = 0xaaaa;
		clrsetbits_le32(&mctl_phy->dx[i].gcr[2], 0xffff, val);

		if (IS_ENABLED(CONFIG_DRAM_ODT_EN))
			val = 0x0;
		else
			val = 0x2020;
		clrsetbits_le32(&mctl_phy->dx[i].gcr[3], 0x3030, val);
	}

	mctl_bit_delay_set(para);
	udelay(1);

	setbits_le32(&mctl_phy->pgcr[6], BIT(0));
	clrbits_le32(&mctl_phy->pgcr[6], 0xfff8);
	for (i = 0; i < 4; i++)
		clrbits_le32(&mctl_phy->dx[i].gcr[3], ~0x3ffff);
	udelay(10);

	if (readl(&mctl_phy->pgsr[1]) & 0xff00000) {
		/* Oops! There's something wrong! */
		panic("Error while initializing DRAM PHY!\n");
	}

	setbits_le32(&mctl_phy->dsgcr, 0xc0);
	clrbits_le32(&mctl_phy->pgcr[1], 0x40);
	clrbits_le32(&mctl_ctl->dfimisc, BIT(0));
	writel(1, &mctl_ctl->swctl);
	mctl_await_completion(&mctl_ctl->swstat, 1, 1);
	clrbits_le32(&mctl_ctl->rfshctl3, BIT(0));

	setbits_le32(&mctl_com->unk_0x014, BIT(31));
	writel(0xffffffff, &mctl_com->maer0);
	writel(0x7ff, &mctl_com->maer1);
	writel(0xffff, &mctl_com->maer2);
}

/* TODO: flexible timing */
static void mctl_set_timing_lpddr3(struct dram_para *para)
{
	struct sunxi_mctl_ctl_reg * const mctl_ctl =
			(struct sunxi_mctl_ctl_reg *)SUNXI_DRAM_CTL0_BASE;
	struct sunxi_mctl_phy_reg * const mctl_phy =
			(struct sunxi_mctl_phy_reg *)SUNXI_DRAM_PHY0_BASE;

	u8 tccd		= 2;
	u8 tfaw		= max(ns_to_t(50), 4);
	u8 trrd		= max(ns_to_t(10), 2);
	u8 trcd		= max(ns_to_t(24), 2);
	u8 trc		= ns_to_t(70);
	u8 txp		= max(ns_to_t(8), 2);
	u8 twtr		= max(ns_to_t(8), 2);
	u8 trtp		= max(ns_to_t(8), 2);
	u8 twr		= max(ns_to_t(15), 3);
	u8 trp		= max(ns_to_t(27), 2);
	u8 tras		= ns_to_t(42);
	u8 twtr_sa	= ns_to_t(5);
	u8 tcksrea	= ns_to_t(11);
	u16 trefi	= ns_to_t(3900) / 32;
	u16 trfc	= ns_to_t(210);

	u8 tmrw		= 5;
	u8 tmrd		= 5;
	u8 tmod		= 12;
	u8 tcke		= 3;
	u8 tcksrx	= 5;
	u8 tcksre	= 5;
	u8 tckesr	= 5;
	u8 trasmax	= 24;
	u8 txs		= 4;
	u8 txsdll	= 4;

	u8 tcl		= 6; /* CL 12 */
	u8 tcwl		= 3; /* CWL 6 */
	u8 t_rdata_en	= 5;

	u32 tdinit0	= (200 * CONFIG_DRAM_CLK) + 1;		/* 200us */
	u32 tdinit1	= (100 * CONFIG_DRAM_CLK) / 1000 + 1;	/* 100ns */
	u32 tdinit2	= (11 * CONFIG_DRAM_CLK) + 1;		/* 11us */
	u32 tdinit3	= (1 * CONFIG_DRAM_CLK) + 1;		/* 1us */

	u8 twtp		= tcwl + 4 + twr + 1;
	u8 twr2rd	= tcwl + 4 + 1 + twtr;
	u8 trd2wr	= tcl + 4 + 5 - tcwl + 1;

	/* set mode register */
	memcpy(mctl_phy->mr, mr_lpddr3, sizeof(mr_lpddr3));

	/* set DRAM timing */
#if 0
	writel((twtp << 24) | (tfaw << 16) | (trasmax << 8) | tras,
	       &mctl_ctl->dramtmg[0]);
	writel((txp << 16) | (trtp << 8) | trc, &mctl_ctl->dramtmg[1]);
	writel((tcwl << 24) | (tcl << 16) | (trd2wr << 8) | twr2rd,
	       &mctl_ctl->dramtmg[2]);
	writel((tmrw << 20) | (tmrd << 12) | tmod, &mctl_ctl->dramtmg[3]);
	writel((trcd << 24) | (tccd << 16) | (trrd << 8) | trp,
	       &mctl_ctl->dramtmg[4]);
	writel((tcksrx << 24) | (tcksre << 16) | (tckesr << 8) | tcke,
	       &mctl_ctl->dramtmg[5]);
	/* Value suggested by ZynqMP manual and used by libdram */
	writel((txp + 2) | 0x02020000, &mctl_ctl->dramtmg[6]);
#else
	memcpy(mctl_ctl->dramtmg, dramtmg_lpddr3, sizeof(dramtmg_lpddr3));
#endif

	clrsetbits_le32(&mctl_ctl->init[0], (3 << 30), (1 << 30));
	writel(0, &mctl_ctl->dfimisc);
	clrsetbits_le32(&mctl_ctl->rankctl, 0xff0, 0x660);

	/*
	 * Set timing registers of the PHY.
	 * Note: the PHY is clocked 2x from the DRAM frequency.
	 */
#if 0
	writel((trrd << 25) | (tras << 17) | (trp << 9) | (trtp << 1),
	       &mctl_phy->dtpr[0]);
	writel((tfaw << 17) | 0x28000400 | (tmrd << 1), &mctl_phy->dtpr[1]);
	writel(((txs << 6) - 1) | (tcke << 17), &mctl_phy->dtpr[2]);
	writel(((txsdll << 22) - (0x1 << 16)) | twtr_sa | (tcksrea << 8),
	       &mctl_phy->dtpr[3]);
	writel((txp << 1) | (trfc << 17) | 0x800, &mctl_phy->dtpr[4]);
	writel((trc << 17) | (trcd << 9) | (twtr << 1), &mctl_phy->dtpr[5]);
	writel(0x0505, &mctl_phy->dtpr[6]);
#else
	memcpy(mctl_phy->dtpr, dtpr_lpddr3, sizeof(dtpr_lpddr3));
#endif

#if 0
	/* Configure DFI timing */
	writel(tcl | 0x2000200 | (t_rdata_en << 16) | 0x808000,
	       &mctl_ctl->dfitmg0);
	writel(0x040201, &mctl_ctl->dfitmg1);

	/* Configure PHY timing */
	writel(tdinit0 | (tdinit1 << 20), &mctl_phy->ptr[3]);
	writel(tdinit2 | (tdinit3 << 18), &mctl_phy->ptr[4]);

	/* set refresh timing */
	writel((trefi << 16) | trfc, &mctl_ctl->rfshtmg);
#else
	writel(0x028a8205, &mctl_ctl->dfitmg0);
	writel(0x00040201, &mctl_ctl->dfitmg1);

	writel(0x42c21590, &mctl_phy->ptr[0]);
	writel(0x682b12c0, &mctl_phy->ptr[1]);
	writel(0x00083def, &mctl_phy->ptr[2]);
	writel(0x04b24541, &mctl_phy->ptr[4]);

	writel(0x002d004f, &mctl_ctl->rfshtmg);
#endif
}

static void mctl_core_init(struct dram_para *para)
{
	mctl_sys_init(para);
	//dump_para((void*)SUNXI_DRAM_PHY0_BASE, 64);
	mctl_com_init(para);
	mctl_set_timing_lpddr3(para);
	mctl_channel_init(0, para);
}

unsigned long sunxi_dram_init(void)
{
	struct sunxi_mctl_com_reg * const mctl_com =
			(struct sunxi_mctl_com_reg *)SUNXI_DRAM_COM_BASE;
	unsigned long size;

	/* RES_CAL_CTRL_REG in BSP U-boot*/
	setbits_le32(0x7010310, BIT(8));
	clrbits_le32(0x7010318, 0x3f);

	//auto_scan_dram_config(&dram_para);
	//dump_para(&dram_para);
	mctl_core_init(&dram_para);
	/* TODO: correctly get size */
	size = 2048;//DRAMC_get_dram_size(&dram_para);
	clrsetbits_le32(&mctl_com->cr, 0xf0, size & 0xf0);
	mctl_set_master_priority();

	return get_ram_size((long *)PHYS_SDRAM_0, PHYS_SDRAM_0_SIZE);
}
