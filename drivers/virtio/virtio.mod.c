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
	{ 0x5a4aa437, "driver_register" },
	{ 0x3098116f, "__bus_register" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xd10d3fe3, "ida_simple_get" },
	{ 0x91715312, "sprintf" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x78b8e152, "device_register" },
	{ 0x7c1372e8, "panic" },
	{ 0x4e86bda8, "ida_simple_remove" },
	{ 0x6894e2b5, "driver_unregister" },
	{ 0xe3338269, "bus_unregister" },
	{ 0xf8915b55, "device_unregister" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x676bbc0f, "_set_bit" },
	{ 0xccc8f905, "dev_set_name" },
	{ 0xe914e41e, "strcpy" },
	{ 0xa7f92105, "add_uevent_var" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

