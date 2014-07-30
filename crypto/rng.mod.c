#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.arch = MODULE_ARCH_INIT,
};

MODULE_INFO(intree, "Y");

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x7f444037, "module_layout" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x79aa04a2, "get_random_bytes" },
	{ 0x21b7a84, "seq_printf" },
	{ 0x156c26cc, "mutex_unlock" },
	{ 0x5770fe32, "nla_put" },
	{ 0x37bf9e0d, "mutex_lock" },
	{ 0x7fd5d85f, "crypto_destroy_tfm" },
	{ 0x37a0cba, "kfree" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x6edc12b3, "crypto_alloc_base" },
	{ 0xe914e41e, "strcpy" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

