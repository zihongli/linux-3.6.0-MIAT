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
	{ 0x1c6f7bd9, "d_path" },
	{ 0x284641b, "device_remove_file" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0xa675804c, "utf8s_to_utf16s" },
	{ 0x7e9efe8e, "complete_and_exit" },
	{ 0x22e1ae6f, "up_read" },
	{ 0x5fc56a46, "_raw_spin_unlock" },
	{ 0xa592f6c9, "dequeue_signal" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x97255bdf, "strlen" },
	{ 0xba230fe, "dev_set_drvdata" },
	{ 0x10785a91, "malloc_sizes" },
	{ 0x20000329, "simple_strtoul" },
	{ 0xf7802486, "__aeabi_uidivmod" },
	{ 0x76cf47f6, "__aeabi_llsl" },
	{ 0x2a3aa678, "_test_and_clear_bit" },
	{ 0x1976aa06, "param_ops_bool" },
	{ 0x57acfb9c, "vfs_fsync" },
	{ 0xa19a0c, "usb_gadget_unregister_driver" },
	{ 0x91715312, "sprintf" },
	{ 0xc6c5a192, "kthread_create_on_node" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x455293f6, "down_read" },
	{ 0x275ef902, "__init_waitqueue_head" },
	{ 0xe707d823, "__aeabi_uidiv" },
	{ 0x1fab5905, "wait_for_completion" },
	{ 0x35b6b772, "param_ops_charp" },
	{ 0x7925a691, "vfs_read" },
	{ 0xfa2a45e, "__memzero" },
	{ 0xcd63c845, "__aeabi_lasr" },
	{ 0xe384a102, "usb_speed_string" },
	{ 0x78b8e152, "device_register" },
	{ 0x11089ac7, "_ctype" },
	{ 0x144a9fae, "dev_err" },
	{ 0x51d559d1, "_raw_spin_unlock_irqrestore" },
	{ 0x1a1431fd, "_raw_spin_unlock_irq" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0x89ff43f6, "init_uts_ns" },
	{ 0x83211609, "up_write" },
	{ 0xa958ed4, "down_write" },
	{ 0x3b653870, "fput" },
	{ 0xd79b5a02, "allow_signal" },
	{ 0x7af8fd78, "device_create_file" },
	{ 0xe7de56e, "_dev_info" },
	{ 0x9f984513, "strrchr" },
	{ 0xfa1d060f, "kmem_cache_alloc" },
	{ 0x2b74c86a, "put_device" },
	{ 0x1000e51, "schedule" },
	{ 0x3507a132, "_raw_spin_lock_irq" },
	{ 0xd26c36e1, "wake_up_process" },
	{ 0xcc5005fe, "msleep_interruptible" },
	{ 0x9c0bd51f, "_raw_spin_lock" },
	{ 0x598542b2, "_raw_spin_lock_irqsave" },
	{ 0x324da471, "usb_gadget_probe_driver" },
	{ 0x37a0cba, "kfree" },
	{ 0x69ad2f20, "kstrtouint" },
	{ 0x9d669763, "memcpy" },
	{ 0x4845c423, "param_array_ops" },
	{ 0x57d85528, "send_sig_info" },
	{ 0x861c55a9, "invalidate_mapping_pages" },
	{ 0xd1b6435a, "dev_warn" },
	{ 0xf8915b55, "device_unregister" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xb742fd7, "simple_strtol" },
	{ 0x676bbc0f, "_set_bit" },
	{ 0xd4669fad, "complete" },
	{ 0xb81960ca, "snprintf" },
	{ 0x99bb8806, "memmove" },
	{ 0xccc8f905, "dev_set_name" },
	{ 0x49ebacbd, "_clear_bit" },
	{ 0x6d044c26, "param_ops_uint" },
	{ 0x8616b4c4, "dev_get_drvdata" },
	{ 0x7d4e2276, "__init_rwsem" },
	{ 0x2b0a2148, "vfs_write" },
	{ 0xaa4dfdea, "filp_open" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

