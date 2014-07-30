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
	{ 0xcaa017c5, "device_remove_bin_file" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x97255bdf, "strlen" },
	{ 0x10785a91, "malloc_sizes" },
	{ 0x89645067, "kobject_uevent" },
	{ 0x8949858b, "schedule_work" },
	{ 0x132a7a5b, "init_timer_key" },
	{ 0x156c26cc, "mutex_unlock" },
	{ 0x91715312, "sprintf" },
	{ 0x649ecf20, "__alloc_pages_nodemask" },
	{ 0x7d11c268, "jiffies" },
	{ 0x275ef902, "__init_waitqueue_head" },
	{ 0x1fab5905, "wait_for_completion" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x6f0036d9, "del_timer_sync" },
	{ 0x76f809e8, "device_del" },
	{ 0x144a9fae, "dev_err" },
	{ 0xdb8a1b3f, "usermodehelper_read_trylock" },
	{ 0x26b708cc, "class_unregister" },
	{ 0x70b072f2, "vmap" },
	{ 0xa8f908f4, "kunmap" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0x37bf9e0d, "mutex_lock" },
	{ 0xc8fd727e, "mod_timer" },
	{ 0xc3f4aec3, "device_add" },
	{ 0x1be03660, "__class_register" },
	{ 0x2d879b44, "contig_page_data" },
	{ 0x7af8fd78, "device_create_file" },
	{ 0x34564d6a, "module_put" },
	{ 0xc6cbbc89, "capable" },
	{ 0xfa1d060f, "kmem_cache_alloc" },
	{ 0x40da4b76, "__free_pages" },
	{ 0xd894ac23, "kmap" },
	{ 0x2b74c86a, "put_device" },
	{ 0x50fad434, "round_jiffies_up" },
	{ 0x1e635bbe, "device_create_bin_file" },
	{ 0x11f57e2a, "__module_get" },
	{ 0x37a0cba, "kfree" },
	{ 0x94961283, "vunmap" },
	{ 0x9d669763, "memcpy" },
	{ 0x793e20e, "device_initialize" },
	{ 0x1db7dc40, "pgprot_kernel" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xb742fd7, "simple_strtol" },
	{ 0x676bbc0f, "_set_bit" },
	{ 0xd4669fad, "complete" },
	{ 0xae69b1c1, "usermodehelper_read_unlock" },
	{ 0xccc8f905, "dev_set_name" },
	{ 0x49ebacbd, "_clear_bit" },
	{ 0xac8f37b2, "outer_cache" },
	{ 0xc03f6a0b, "try_module_get" },
	{ 0x2d1b02d2, "usermodehelper_read_lock_wait" },
	{ 0xe914e41e, "strcpy" },
	{ 0xa7f92105, "add_uevent_var" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

