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
	{ 0x6ea42e80, "release_sock" },
	{ 0x824b1b85, "sock_init_data" },
	{ 0x642d81c9, "sock_no_setsockopt" },
	{ 0x1d72b68c, "sock_no_getsockopt" },
	{ 0x8fa033a3, "sock_no_ioctl" },
	{ 0x10785a91, "malloc_sizes" },
	{ 0x6fd1a3b2, "sock_queue_rcv_skb" },
	{ 0xcf3295cb, "skb_recv_datagram" },
	{ 0x156c26cc, "mutex_unlock" },
	{ 0x121e33f8, "rpmsg_send_offchannel_raw" },
	{ 0xa25b4818, "sock_no_mmap" },
	{ 0xd8b4ee06, "sock_no_socketpair" },
	{ 0xb58222c2, "sk_alloc" },
	{ 0x144a9fae, "dev_err" },
	{ 0x27e1a049, "printk" },
	{ 0x12e003f7, "lock_sock_nested" },
	{ 0xbbd2db3c, "unregister_rpmsg_driver" },
	{ 0x4381c2d4, "sock_no_listen" },
	{ 0x37bf9e0d, "mutex_lock" },
	{ 0x7ec27df5, "sock_no_accept" },
	{ 0xc821a795, "sk_free" },
	{ 0x5d5b5a16, "radix_tree_delete" },
	{ 0x5a9190a8, "sock_no_shutdown" },
	{ 0x4059792f, "print_hex_dump" },
	{ 0xe7e8b61a, "proto_register" },
	{ 0xfa1d060f, "kmem_cache_alloc" },
	{ 0x4a9e7409, "register_rpmsg_driver" },
	{ 0xcd213c5a, "sock_register" },
	{ 0xe87a42b0, "kfree_skb" },
	{ 0x8d52867e, "proto_unregister" },
	{ 0x762c53dd, "sock_alloc_send_skb" },
	{ 0xe7c24e9, "skb_copy_datagram_iovec" },
	{ 0x5872dbf7, "get_virtproc_id" },
	{ 0x37a0cba, "kfree" },
	{ 0x9d669763, "memcpy" },
	{ 0x62737e1d, "sock_unregister" },
	{ 0x9fb3dd30, "memcpy_fromiovec" },
	{ 0xddae80df, "rpmsg_create_channel" },
	{ 0x870bf928, "radix_tree_lookup" },
	{ 0xd1b6435a, "dev_warn" },
	{ 0xf8915b55, "device_unregister" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x200913e, "skb_put" },
	{ 0xf202c5cb, "radix_tree_insert" },
	{ 0x66259082, "skb_free_datagram" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=virtio_rpmsg_bus";

