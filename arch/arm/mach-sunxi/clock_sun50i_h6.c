#include <common.h>
#include <asm/io.h>
#include <asm/arch/cpu.h>
#include <asm/arch/clock.h>

#ifdef CONFIG_SPL_BUILD
void clock_init_safe(void)
{
	clock_set_pll1(408000000);
}
#endif

void clock_init_uart(void)
{
	struct sunxi_ccm_reg *const ccm =
		(struct sunxi_ccm_reg *)SUNXI_CCM_BASE;

	/* uart clock source is apb2 */
	writel(APB2_CLK_SRC_OSC24M|
	       APB2_CLK_RATE_N_1|
	       APB2_CLK_RATE_M(1),
	       &ccm->apb2_cfg);

	/* open the clock for uart */
	setbits_le32(&ccm->uart_gate_reset,
		     1 << (CONFIG_CONS_INDEX - 1));

	/* deassert uart reset */
	setbits_le32(&ccm->uart_gate_reset,
		     1 << (RESET_SHIFT + CONFIG_CONS_INDEX - 1));
}

#ifdef CONFIG_SPL_BUILD
void clock_set_pll1(unsigned int clk)
{
	struct sunxi_ccm_reg * const ccm =
		(struct sunxi_ccm_reg *)SUNXI_CCM_BASE;
	u32 val;

	/* Do not support clocks < 288MHz as they need factor P */
	if (clk < 288000000) clk = 288000000;

	/* Switch to 24MHz clock while changing PLL1 */
	val = readl(&ccm->cpu_axi_cfg);
	val &= ~CCM_CPU_AXI_MUX_MASK;
	val |= CCM_CPU_AXI_MUX_OSC24M;
	writel(val, &ccm->cpu_axi_cfg);

	/* clk = 24*n/p, p is ignored if clock is >288MHz */
	writel(CCM_PLL1_CTRL_EN | CCM_PLL1_LOCK_EN | CCM_PLL1_CLOCK_TIME_2 |
	       CCM_PLL1_CTRL_N(clk / 24000000), &ccm->pll1_cfg);
	while (!(readl(&ccm->pll1_cfg) & CCM_PLL1_LOCK)) {}

	/* Switch CPU to PLL1 */
	val = readl(&ccm->cpu_axi_cfg);
	val &= ~CCM_CPU_AXI_MUX_MASK;
	val |= CCM_CPU_AXI_MUX_PLL_CPUX;
	writel(val, &ccm->cpu_axi_cfg);
}
#endif

void clock_set_pll3(unsigned int clk)
{
	struct sunxi_ccm_reg * const ccm =
		(struct sunxi_ccm_reg *)SUNXI_CCM_BASE;
	const int m = 1; /* 3 MHz steps just like sun4i, sun5i and sun7i */

	if (clk == 0) {
		clrbits_le32(&ccm->pll3_cfg, CCM_PLL3_CTRL_EN);
		return;
	}

	/* PLL rate = 24000000 * n / m / 4 */
	writel(CCM_PLL3_CTRL_EN | CCM_PLL3_CTRL_N(clk / (24000000 / 4 / m)) |
	       CCM_PLL3_CTRL_M(m), &ccm->pll3_cfg);
}

void clock_set_pll3_factors(int m, int n)
{
	struct sunxi_ccm_reg * const ccm =
		(struct sunxi_ccm_reg *)SUNXI_CCM_BASE;

	/* PLL rate = 6000000 * n / m */
	writel(CCM_PLL3_CTRL_EN | CCM_PLL3_CTRL_N(n) |
	       CCM_PLL3_CTRL_M(m), &ccm->pll3_cfg);
}

unsigned int clock_get_pll3(void)
{
	struct sunxi_ccm_reg *const ccm =
		(struct sunxi_ccm_reg *)SUNXI_CCM_BASE;
	uint32_t rval = readl(&ccm->pll3_cfg);
	int n = ((rval & CCM_PLL3_CTRL_N_MASK) >> CCM_PLL3_CTRL_N_SHIFT) + 1;
	int m = ((rval & CCM_PLL3_CTRL_M_MASK) >> CCM_PLL3_CTRL_M_SHIFT) + 1;

	/* Multiply by 1000 after dividing by m to avoid integer overflows */
	return (6000 * n / m) * 1000;
}

unsigned int clock_get_pll6(void)
{
	struct sunxi_ccm_reg *const ccm =
		(struct sunxi_ccm_reg *)SUNXI_CCM_BASE;

	uint32_t rval = readl(&ccm->pll6_cfg);
	int n = ((rval & CCM_PLL6_CTRL_N_MASK) >> CCM_PLL6_CTRL_N_SHIFT);
	int div1 = ((rval & CCM_PLL6_CTRL_DIV1_MASK) >>
			CCM_PLL6_CTRL_DIV1_SHIFT) + 1;
	int div2 = ((rval & CCM_PLL6_CTRL_DIV2_MASK) >>
			CCM_PLL6_CTRL_DIV2_SHIFT) + 1;
	/* The register defines PLL6-4X, not plain PLL6 */
	return 24000000 * n / div1 / div2 / 4;
}
