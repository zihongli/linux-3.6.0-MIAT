/*
 * This file contains common code that is intended to be used across
 * boards so that it's not replicated.
 *
 *  Copyright (C) 2011 Xilinx
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/cpumask.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/opp.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/of.h>
#include <linux/memblock.h>
#include <linux/dma-mapping.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/time.h>
#include <asm/mach-types.h>
#include <asm/page.h>
#include <asm/hardware/gic.h>
#include <asm/hardware/cache-l2x0.h>

#include <mach/zynq_soc.h>
#include <mach/clk.h>
#include <mach/pdev.h>
#include "common.h"

static struct of_device_id zynq_of_bus_ids[] __initdata = {
	{ .compatible = "simple-bus", },
	{}
};

static const struct of_device_id zynq_dt_irq_match[] __initconst = {
	{ .compatible = "arm,cortex-a9-gic", .data = gic_of_init },
	{ }
};

/* The minimum devices needed to be mapped before the VM system is up and
 * running include the GIC, UART and Timer Counter.
 */
static struct map_desc io_desc[] __initdata = {
	{
		.virtual	= SCU_PERIPH_VIRT,
		.pfn		= __phys_to_pfn(SCU_PERIPH_PHYS),
		.length		= SZ_8K,
		.type		= MT_DEVICE,
	},

#ifdef CONFIG_DEBUG_LL
	{
		.virtual	= LL_UART_VADDR,
		.pfn		= __phys_to_pfn(LL_UART_PADDR),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	},
#endif

	/* SLCR space for clock stuff for now */
	{
		.virtual	= SLCR_BASE_VIRT,
		.pfn		= __phys_to_pfn(SLCR_BASE_PHYS),
		.length		= (3 * SZ_1K),
		.type		= MT_DEVICE,
	},
};

static void __init xilinx_zynq_timer_init(void)
{
	xttcpss_timer_init();
}

/*
 * Instantiate and initialize the system timer structure
 */
struct sys_timer xttcpss_sys_timer = {
	.init		= xilinx_zynq_timer_init,
};

/**
 * xilinx_map_io() - Create memory mappings needed for early I/O.
 */
void __init xilinx_map_io(void)
{
	iotable_init(io_desc, ARRAY_SIZE(io_desc));
	init_consistent_dma_size(40 << 20);
}

/**
 * xilinx_memory_init() - Initialize special memory
 *
 * We need to stop things allocating the low memory as DMA can't work in
 * the 1st 512K of memory.  Using reserve vs remove is not totally clear yet.
 */
void __init xilinx_memory_init()
{
#if (CONFIG_PHYS_OFFSET == 0)
	/* Reserve the 0-0x4000 addresses (before page tables and kernel)
	 * which can't be used for DMA
	 */
	memblock_reserve(0, 0x4000);
#endif
}

#ifdef CONFIG_CPU_FREQ
#define CPUFREQ_MIN_FREQ_HZ	200000000
static unsigned int freq_divs[] __initdata = {
	2, 3
};

/**
 * xilinx_opp_init() - Register OPPs
 *
 * Registering frequency/voltage operating points for voltage and frequency
 * scaling. Currently we only support frequency scaling.
 */
static void __init xilinx_opp_init(void)
{
	struct platform_device *pdev = xilinx_get_pdev_by_name("zynq-dvfs");
	struct device *dev;
	int ret = 0;
	long freq;
	struct clk *cpuclk = clk_get_sys("CPU_6OR4X_CLK", NULL);
	int i;

	if (IS_ERR(pdev)) {
		pr_warn("Xilinx OOP init: No device. DVFS not available.");
		return;
	}
	dev = &pdev->dev;

	if (IS_ERR(cpuclk)) {
		pr_warn("Xilinx OOP init: CPU clock not found. DVFS not available.");
		return;
	}

	/* frequency/voltage operating points. For now use f only */
	freq = clk_get_rate(cpuclk);
	ret |= opp_add(dev, freq, 0);
	for (i = 0; i < ARRAY_SIZE(freq_divs); i++) {
		long tmp = clk_round_rate(cpuclk, freq / freq_divs[i]);
		if (tmp >= CPUFREQ_MIN_FREQ_HZ)
			ret |= opp_add(dev, tmp, 0);
	}
	freq = clk_round_rate(cpuclk, CPUFREQ_MIN_FREQ_HZ);
	if (freq >= CPUFREQ_MIN_FREQ_HZ && IS_ERR(opp_find_freq_exact(dev, freq,
				1)))
		ret |= opp_add(dev, freq, 0);

	if (ret)
		pr_warn("Error adding OPPs.");
}
#else
static void __init xilinx_opp_init(void) {}
#endif

#ifdef CONFIG_CACHE_L2X0
static int __init xilinx_l2c_init(void)
{
	/* 64KB way size, 8-way associativity, parity disabled,
	 * prefetching option */
#ifndef	CONFIG_XILINX_L2_PREFETCH
	return l2x0_of_init(0x02060000, 0xF0F0FFFF);
#else
	return l2x0_of_init(0x72060000, 0xF0F0FFFF);
#endif
}
early_initcall(xilinx_l2c_init);
#endif

/**
 * xilinx_irq_init() - Interrupt controller initialization for the GIC.
 */
void __init xilinx_irq_init(void)
{
	of_irq_init(zynq_dt_irq_match);
	/* This is probably the ugliest hack possible but this is why:
	 * Clock init needs to be done before timer init, so the timer can use
	 * COMMON_CLK. All __initcall types are called after time_init().
	 * Putting it in here is ugly but works. */
	zynq_clock_init();
}

/**
 * xilinx_init_machine() - System specific initialization, intended to be
 *			   called from board specific initialization.
 */
void __init xilinx_init_machine(void)
{
	of_platform_bus_probe(NULL, zynq_of_bus_ids, NULL);
	platform_device_init();
	xilinx_opp_init();
}
