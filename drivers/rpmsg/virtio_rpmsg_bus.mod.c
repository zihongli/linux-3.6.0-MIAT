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
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0xa6e1f7c3, "arm_dma_ops" },
	{ 0x10785a91, "malloc_sizes" },
	{ 0x7be9ce4d, "virtio_check_driver_offered_feature" },
	{ 0x156c26cc, "mutex_unlock" },
	{ 0x3338f9f2, "virtqueue_kick" },
	{ 0x91715312, "sprintf" },
	{ 0xb523a0c4, "virtqueue_get_buf" },
	{ 0x275ef902, "__init_waitqueue_head" },
	{ 0x5f754e5a, "memset" },
	{ 0x23c08068, "idr_destroy" },
	{ 0x78b8e152, "device_register" },
	{ 0x144a9fae, "dev_err" },
	{ 0x128dc84b, "__mutex_init" },
	{ 0x6433ebff, "device_find_child" },
	{ 0x27e1a049, "printk" },
	{ 0x45b24084, "virtqueue_add_buf" },
	{ 0x6894e2b5, "driver_unregister" },
	{ 0x328a05f1, "strncpy" },
	{ 0xd52dd30c, "virtqueue_disable_cb" },
	{ 0x84b183ae, "strncmp" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0x37bf9e0d, "mutex_lock" },
	{ 0xe3338269, "bus_unregister" },
	{ 0x3bd04a33, "idr_remove" },
	{ 0xa41178a8, "idr_pre_get" },
	{ 0x4059792f, "print_hex_dump" },
	{ 0xe7de56e, "_dev_info" },
	{ 0xfa1d060f, "kmem_cache_alloc" },
	{ 0xd0a8f4a4, "idr_remove_all" },
	{ 0x3791df5d, "unregister_virtio_driver" },
	{ 0x2b74c86a, "put_device" },
	{ 0x3bd1b1f6, "msecs_to_jiffies" },
	{ 0xd62c833f, "schedule_timeout" },
	{ 0xf22e446b, "idr_get_new_above" },
	{ 0xd85cd67e, "__wake_up" },
	{ 0xc4a58682, "device_for_each_child" },
	{ 0xefdd2345, "sg_init_one" },
	{ 0x37a0cba, "kfree" },
	{ 0x9d669763, "memcpy" },
	{ 0x499cb58c, "prepare_to_wait" },
	{ 0x1cfb04fa, "finish_wait" },
	{ 0xd1b6435a, "dev_warn" },
	{ 0xf8915b55, "device_unregister" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xccc8f905, "dev_set_name" },
	{ 0xfbfb179d, "idr_init" },
	{ 0x9417d022, "idr_find" },
	{ 0x1d8caeee, "idr_get_new" },
	{ 0x5b4352b8, "virtqueue_enable_cb" },
	{ 0xa7f92105, "add_uevent_var" },
	{ 0x33b5aae4, "register_virtio_driver" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=virtio,virtio_ring";

MODULE_ALIAS("virtio:d00000007v*");
