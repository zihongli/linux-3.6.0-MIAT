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
	{ 0x284641b, "device_remove_file" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0xa675804c, "utf8s_to_utf16s" },
	{ 0x5fc56a46, "_raw_spin_unlock" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xcca27eeb, "del_timer" },
	{ 0x97255bdf, "strlen" },
	{ 0xba230fe, "dev_set_drvdata" },
	{ 0x10785a91, "malloc_sizes" },
	{ 0x20000329, "simple_strtoul" },
	{ 0xf7802486, "__aeabi_uidivmod" },
	{ 0x1976aa06, "param_ops_bool" },
	{ 0x132a7a5b, "init_timer_key" },
	{ 0xa19a0c, "usb_gadget_unregister_driver" },
	{ 0x91715312, "sprintf" },
	{ 0x7d11c268, "jiffies" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x35b6b772, "param_ops_charp" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x6f0036d9, "del_timer_sync" },
	{ 0x5f754e5a, "memset" },
	{ 0xe384a102, "usb_speed_string" },
	{ 0x11089ac7, "_ctype" },
	{ 0x144a9fae, "dev_err" },
	{ 0x51d559d1, "_raw_spin_unlock_irqrestore" },
	{ 0x27e1a049, "printk" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0xc8fd727e, "mod_timer" },
	{ 0x89ff43f6, "init_uts_ns" },
	{ 0x7af8fd78, "device_create_file" },
	{ 0xe7de56e, "_dev_info" },
	{ 0x9f984513, "strrchr" },
	{ 0xfa1d060f, "kmem_cache_alloc" },
	{ 0x9c0bd51f, "_raw_spin_lock" },
	{ 0x598542b2, "_raw_spin_lock_irqsave" },
	{ 0x1e047854, "warn_slowpath_fmt" },
	{ 0x324da471, "usb_gadget_probe_driver" },
	{ 0x37a0cba, "kfree" },
	{ 0x9d669763, "memcpy" },
	{ 0xd1b6435a, "dev_warn" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x676bbc0f, "_set_bit" },
	{ 0xb81960ca, "snprintf" },
	{ 0x487d9343, "param_ops_ushort" },
	{ 0x6d044c26, "param_ops_uint" },
	{ 0x8616b4c4, "dev_get_drvdata" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

