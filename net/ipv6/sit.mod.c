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
	{ 0xa65be582, "xfrm4_tunnel_register" },
	{ 0xd21712b, "register_pernet_device" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x60a13e90, "rcu_barrier" },
	{ 0x1a0e5555, "unregister_pernet_device" },
	{ 0x3b6ddc3e, "xfrm4_tunnel_deregister" },
	{ 0x10785a91, "malloc_sizes" },
	{ 0xfa1d060f, "kmem_cache_alloc" },
	{ 0x9469482, "kfree_call_rcu" },
	{ 0xbc10dd97, "__put_user_4" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x98082893, "__copy_to_user" },
	{ 0xc0917f1f, "netdev_state_change" },
	{ 0x609f1c7e, "synchronize_net" },
	{ 0xc6cbbc89, "capable" },
	{ 0x17a142df, "__copy_from_user" },
	{ 0x1381644c, "register_netdevice" },
	{ 0x73e20c1c, "strlcpy" },
	{ 0x32ddd5b6, "call_rcu" },
	{ 0x6b2dc060, "dump_stack" },
	{ 0x27e1a049, "printk" },
	{ 0x85670f1d, "rtnl_is_locked" },
	{ 0x59e8ca9, "ipv4_redirect" },
	{ 0xe7ec86e7, "ipv4_update_pmtu" },
	{ 0x3838e81a, "ipv6_chk_prefix" },
	{ 0xe87a42b0, "kfree_skb" },
	{ 0xf887172a, "netif_rx" },
	{ 0x3dca1a52, "__secpath_destroy" },
	{ 0x6cb79c2d, "__pskb_pull_tail" },
	{ 0x41930374, "register_netdev" },
	{ 0x50c89f23, "__alloc_percpu" },
	{ 0xe914e41e, "strcpy" },
	{ 0xd044be70, "alloc_netdev_mqs" },
	{ 0x247ba655, "free_netdev" },
	{ 0xc9ec4e21, "free_percpu" },
	{ 0xba3b3693, "sock_wfree" },
	{ 0x7d11c268, "jiffies" },
	{ 0x61526fc9, "ip_local_out" },
	{ 0x13d457ba, "__ip_select_ident" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x852d599, "skb_push" },
	{ 0x19ccfeeb, "consume_skb" },
	{ 0x9d395e62, "skb_realloc_headroom" },
	{ 0x7c63267, "icmpv6_send" },
	{ 0x145fe89a, "neigh_destroy" },
	{ 0xd542439, "__ipv6_addr_type" },
	{ 0xf6ebc03b, "net_ratelimit" },
	{ 0x37a0cba, "kfree" },
	{ 0x14e3a12f, "init_net" },
	{ 0xcde64679, "__dev_get_by_index" },
	{ 0xec56ab61, "dst_release" },
	{ 0xabd2bcca, "ip_route_output_flow" },
	{ 0x341dbfa3, "__per_cpu_offset" },
	{ 0xfe7c4287, "nr_cpu_ids" },
	{ 0xd3e6f60d, "cpu_possible_mask" },
	{ 0xd3f57a2, "_find_next_bit_le" },
	{ 0x6e720ff2, "rtnl_unlock" },
	{ 0x9fdecc31, "unregister_netdevice_many" },
	{ 0xc7a4fbed, "rtnl_lock" },
	{ 0x2469810f, "__rcu_read_unlock" },
	{ 0x8d522714, "__rcu_read_lock" },
	{ 0xa02f554f, "unregister_netdevice_queue" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=tunnel4,ipv6";

