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
	{ 0x609f1c7e, "synchronize_net" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x685ed4c4, "icmp_send" },
	{ 0x156c26cc, "mutex_unlock" },
	{ 0x2ea6477f, "inet_del_protocol" },
	{ 0x6cb79c2d, "__pskb_pull_tail" },
	{ 0x27e1a049, "printk" },
	{ 0x37bf9e0d, "mutex_lock" },
	{ 0xc41db819, "inet_add_protocol" },
	{ 0xe87a42b0, "kfree_skb" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

