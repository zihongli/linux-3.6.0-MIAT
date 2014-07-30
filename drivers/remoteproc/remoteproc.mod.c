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
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x97d7b25c, "vring_del_virtqueue" },
	{ 0x528c709d, "simple_read_from_buffer" },
	{ 0x8e9481da, "generic_file_llseek" },
	{ 0xe0306161, "debugfs_create_dir" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xa6e1f7c3, "arm_dma_ops" },
	{ 0x10785a91, "malloc_sizes" },
	{ 0xd20bed67, "register_virtio_device" },
	{ 0x156c26cc, "mutex_unlock" },
	{ 0xd10d3fe3, "ida_simple_get" },
	{ 0xf573eb92, "debugfs_create_file" },
	{ 0x293d1a0d, "debugfs_remove_recursive" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x81f3afb, "complete_all" },
	{ 0x275ef902, "__init_waitqueue_head" },
	{ 0x1fab5905, "wait_for_completion" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x23c08068, "idr_destroy" },
	{ 0x76f809e8, "device_del" },
	{ 0x144a9fae, "dev_err" },
	{ 0x33b80125, "mutex_lock_interruptible" },
	{ 0x128dc84b, "__mutex_init" },
	{ 0x27e1a049, "printk" },
	{ 0x71c90087, "memcmp" },
	{ 0x4e86bda8, "ida_simple_remove" },
	{ 0xb8250f4b, "debugfs_remove" },
	{ 0xc3f4aec3, "device_add" },
	{ 0xa7593d11, "simple_open" },
	{ 0xdaaa1197, "request_firmware_nowait" },
	{ 0x6fa6b7f2, "unregister_virtio_device" },
	{ 0x3bd04a33, "idr_remove" },
	{ 0xa41178a8, "idr_pre_get" },
	{ 0x34564d6a, "module_put" },
	{ 0x92a20735, "vring_new_virtqueue" },
	{ 0xe7de56e, "_dev_info" },
	{ 0xfa1d060f, "kmem_cache_alloc" },
	{ 0xd0a8f4a4, "idr_remove_all" },
	{ 0xd9ce8f0c, "strnlen" },
	{ 0x2b74c86a, "put_device" },
	{ 0xff05fa13, "vring_interrupt" },
	{ 0x716265c7, "debugfs_initialized" },
	{ 0xde7963c4, "get_device" },
	{ 0x37a0cba, "kfree" },
	{ 0x9d669763, "memcpy" },
	{ 0x793e20e, "device_initialize" },
	{ 0xcfed290, "request_firmware" },
	{ 0xd1b6435a, "dev_warn" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xb81960ca, "snprintf" },
	{ 0xccc8f905, "dev_set_name" },
	{ 0xfbfb179d, "idr_init" },
	{ 0x9417d022, "idr_find" },
	{ 0x1d8caeee, "idr_get_new" },
	{ 0xd215f5, "vring_transport_features" },
	{ 0xa5c71cd5, "release_firmware" },
	{ 0xc03f6a0b, "try_module_get" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=virtio_ring,virtio,firmware_class";

