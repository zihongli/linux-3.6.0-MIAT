/*
 * CPU frequency scaling for Zynq
 *
 * Based on drivers/cpufreq/omap-cpufreq.c,
 * Copyright (C) 2005 Nokia Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/kernel.h>
#include <linux/cpufreq.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/cpu.h>
#include <linux/cpumask.h>
#include <linux/module.h>
#include <linux/opp.h>
#include <linux/platform_device.h>
#include <asm/smp_plat.h>
#include <asm/cpu.h>
#include <mach/pdev.h>

#ifdef CONFIG_SMP
struct lpj_info {
	unsigned long	ref;
	unsigned int	freq;
};

static DEFINE_PER_CPU(struct lpj_info, lpj_ref);
static struct lpj_info global_lpj_ref;
#endif

static atomic_t freq_table_users = ATOMIC_INIT(0);
static struct cpufreq_frequency_table *freq_table;
static struct device *mpu_dev;
static struct clk *cpuclk;

static int zynq_verify_speed(struct cpufreq_policy *policy)
{
	if (!freq_table)
		return -EINVAL;
	return cpufreq_frequency_table_verify(policy, freq_table);
}

static unsigned int zynq_getspeed(unsigned int cpu)
{
	unsigned long rate;

	if (cpu >= num_present_cpus())
		return 0;

	rate = clk_get_rate(cpuclk) / 1000;
	return rate;
}

static int zynq_target(struct cpufreq_policy *policy,
		       unsigned int target_freq,
		       unsigned int relation)
{
	unsigned int i;
	int ret = 0;
	struct cpufreq_freqs freqs;
	unsigned long freq, volt = 0, volt_old = 0;

	if (!freq_table) {
		dev_err(mpu_dev, "%s: cpu%d: no freq table!\n", __func__,
				policy->cpu);
		return -EINVAL;
	}

	ret = cpufreq_frequency_table_target(policy, freq_table, target_freq,
			relation, &i);
	if (ret) {
		dev_dbg(mpu_dev, "%s: cpu%d: no freq match for %d(ret=%d)\n",
			__func__, policy->cpu, target_freq, ret);
		return ret;
	}
	freqs.new = freq_table[i].frequency;
	if (!freqs.new) {
		dev_err(mpu_dev, "%s: cpu%d: no match for freq %d\n", __func__,
			policy->cpu, target_freq);
		return -EINVAL;
	}

	freqs.old = zynq_getspeed(policy->cpu);
	freqs.cpu = policy->cpu;

	if (freqs.old == freqs.new && policy->cur == freqs.new)
		return ret;

	/* notifiers */
	for_each_cpu(i, policy->cpus) {
		freqs.cpu = i;
		cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);
	}

	freq = freqs.new * 1000;

	dev_dbg(mpu_dev, "cpufreq-zynq: %u MHz, %ld mV --> %u MHz, %ld mV\n",
		freqs.old / 1000, volt_old ? volt_old / 1000 : -1,
		freqs.new / 1000, volt ? volt / 1000 : -1);

	ret = clk_set_rate(cpuclk, freqs.new * 1000);

	freqs.new = zynq_getspeed(policy->cpu);
#ifdef CONFIG_SMP
	/*
	 * Note that loops_per_jiffy is not updated on SMP systems in
	 * cpufreq driver. So, update the per-CPU loops_per_jiffy value
	 * on frequency transition. We need to update all dependent CPUs.
	 */
	for_each_cpu(i, policy->cpus) {
		struct lpj_info *lpj = &per_cpu(lpj_ref, i);
		if (!lpj->freq) {
			lpj->ref = per_cpu(cpu_data, i).loops_per_jiffy;
			lpj->freq = freqs.old;
		}

		per_cpu(cpu_data, i).loops_per_jiffy =
			cpufreq_scale(lpj->ref, lpj->freq, freqs.new);
	}

	/* And don't forget to adjust the global one */
	if (!global_lpj_ref.freq) {
		global_lpj_ref.ref = loops_per_jiffy;
		global_lpj_ref.freq = freqs.old;
	}
	loops_per_jiffy = cpufreq_scale(global_lpj_ref.ref, global_lpj_ref.freq,
					freqs.new);
#endif

	/* notifiers */
	for_each_cpu(i, policy->cpus) {
		freqs.cpu = i;
		cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);
	}

	return ret;
}

static inline void freq_table_free(void)
{
	if (atomic_dec_and_test(&freq_table_users))
		opp_free_cpufreq_table(mpu_dev, &freq_table);
}

static int __cpuinit zynq_cpu_init(struct cpufreq_policy *policy)
{
	int result = 0;

	cpuclk = clk_get_sys("CPU_6OR4X_CLK", NULL);
	if (IS_ERR(cpuclk)) {
		pr_warn("Xilinx: cpufreq: Clock not found.");
		return PTR_ERR(cpuclk);
	}

	if (policy->cpu >= num_possible_cpus()) {
		result = -EINVAL;
		goto fail_ck;
	}

	policy->cur = policy->min = policy->max = zynq_getspeed(policy->cpu);

	if (!freq_table)
		result = opp_init_cpufreq_table(mpu_dev, &freq_table);

	if (result) {
		dev_err(mpu_dev, "%s: cpu%d: failed creating freq table[%d]\n",
				__func__, policy->cpu, result);
		goto fail_ck;
	}

	atomic_inc(&freq_table_users);

	result = cpufreq_frequency_table_cpuinfo(policy, freq_table);
	if (result)
		goto fail_table;

	cpufreq_frequency_table_get_attr(freq_table, policy->cpu);

	policy->min = policy->cpuinfo.min_freq;
	policy->max = policy->cpuinfo.max_freq;
	policy->cur = zynq_getspeed(policy->cpu);

	/*
	 * On Zynq configuartion, both processors share the voltage
	 * and clock. So both CPUs needs to be scaled together and hence
	 * needs software co-ordination. Use cpufreq affected_cpus
	 * interface to handle this scenario. Additional is_smp() check
	 * is to keep SMP_ON_UP build working.
	 */
	if (is_smp()) {
		policy->shared_type = CPUFREQ_SHARED_TYPE_ANY;
		cpumask_setall(policy->cpus);
	}

	/* FIXME: what's the actual transition time? */
	policy->cpuinfo.transition_latency = 300 * 1000;

	return 0;

fail_table:
	freq_table_free();
fail_ck:
	clk_put(cpuclk);
	return result;
}

static int zynq_cpu_exit(struct cpufreq_policy *policy)
{
	clk_put(cpuclk);
	return 0;
}

static struct freq_attr *zynq_cpufreq_attr[] = {
	&cpufreq_freq_attr_scaling_available_freqs,
	NULL,
};

static struct cpufreq_driver zynq_cpufreq_driver = {
	.flags		= CPUFREQ_STICKY,
	.verify		= zynq_verify_speed,
	.target		= zynq_target,
	.get		= zynq_getspeed,
	.init		= zynq_cpu_init,
	.exit		= zynq_cpu_exit,
	.name		= "Zynq cpufreq",
	.attr		= zynq_cpufreq_attr,
};

static int __init zynq_cpufreq_init(void)
{
	struct platform_device *pdev = xilinx_get_pdev_by_name("zynq-dvfs");

	if (IS_ERR(pdev)) {
		pr_warn("Xilinx: cpufreq: Device not found.");
		return -EINVAL;
	}
	mpu_dev = &pdev->dev;
	return cpufreq_register_driver(&zynq_cpufreq_driver);
}

static void __exit zynq_cpufreq_exit(void)
{
	cpufreq_unregister_driver(&zynq_cpufreq_driver);
}

MODULE_DESCRIPTION("cpufreq driver for Zynq");
MODULE_LICENSE("GPL");
module_init(zynq_cpufreq_init);
module_exit(zynq_cpufreq_exit);
