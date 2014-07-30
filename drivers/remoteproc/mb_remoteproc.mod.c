#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

MODULE_INFO(intree, "Y");

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x7f444037, "module_layout" },
	{ 0x35b6b772, "param_ops_charp" },
	{ 0x534cad44, "platform_driver_unregister" },
	{ 0xb339b217, "platform_driver_register" },
	{ 0x10785a91, "malloc_sizes" },
	{ 0x24d70598, "rproc_add" },
	{ 0x367f99d6, "rproc_alloc" },
	{ 0x40a6f522, "__arm_ioremap" },
	{ 0x6f0553bd, "of_get_property" },
	{ 0x2072ee9b, "request_threaded_irq" },
	{ 0x1ee9814e, "irq_of_parse_and_map" },
	{ 0x6f784465, "of_irq_count" },
	{ 0xea06e86f, "dma_supported" },
	{ 0xbc2cc57a, "dma_declare_coherent_memory" },
	{ 0x878dd66b, "platform_get_resource" },
	{ 0xba230fe, "dev_set_drvdata" },
	{ 0xfa1d060f, "kmem_cache_alloc" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x144a9fae, "dev_err" },
	{ 0xe4972206, "rproc_put" },
	{ 0x6a73ef26, "rproc_del" },
	{ 0xf8ae9ce2, "dma_release_declared_memory" },
	{ 0x37a0cba, "kfree" },
	{ 0xf20dabd8, "free_irq" },
	{ 0xac8f37b2, "outer_cache" },
	{ 0x23f7ee8d, "cpu_cache" },
	{ 0x8616b4c4, "dev_get_drvdata" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xe7de56e, "_dev_info" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=remoteproc";

MODULE_ALIAS("of:N*T*Cxlnx,mb_remoteproc*");
