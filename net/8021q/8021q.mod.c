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
	{ 0x1381644c, "register_netdevice" },
	{ 0xae172228, "proc_net_mkdir" },
	{ 0x17a142df, "__copy_from_user" },
	{ 0xfd9c554c, "dev_change_flags" },
	{ 0x582dfe24, "dev_mc_unsync" },
	{ 0x703fe398, "single_open" },
	{ 0x98082893, "__copy_to_user" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x60a13e90, "rcu_barrier" },
	{ 0xa6c7162, "vlan_dev_vlan_id" },
	{ 0xb1fc3391, "dev_uc_add" },
	{ 0x6203650c, "single_release" },
	{ 0x10785a91, "malloc_sizes" },
	{ 0x8da224bb, "seq_puts" },
	{ 0xc7a4fbed, "rtnl_lock" },
	{ 0xdc393998, "netif_carrier_on" },
	{ 0xd3f57a2, "_find_next_bit_le" },
	{ 0x21b7a84, "seq_printf" },
	{ 0x63ecad53, "register_netdevice_notifier" },
	{ 0xa016500f, "netif_carrier_off" },
	{ 0x84aa7ce7, "remove_proc_entry" },
	{ 0x50c89f23, "__alloc_percpu" },
	{ 0x983281e3, "dev_set_allmulti" },
	{ 0xdc4dacb5, "vlan_vid_del" },
	{ 0xea32c910, "call_netdevice_notifiers" },
	{ 0x4f7d687f, "linkwatch_fire_event" },
	{ 0xa4871250, "vlan_vid_add" },
	{ 0x46477288, "seq_read" },
	{ 0xc9ec4e21, "free_percpu" },
	{ 0xfe769456, "unregister_netdevice_notifier" },
	{ 0xce5e144a, "vlan_ioctl_set" },
	{ 0xfe7c4287, "nr_cpu_ids" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x6590666d, "unregister_pernet_subsys" },
	{ 0x9fdecc31, "unregister_netdevice_many" },
	{ 0x27e1a049, "printk" },
	{ 0x3a6ddb35, "ethtool_op_get_link" },
	{ 0x84943565, "proc_net_remove" },
	{ 0x5c12906d, "__ethtool_get_settings" },
	{ 0x247ba655, "free_netdev" },
	{ 0x328a05f1, "strncpy" },
	{ 0x5770fe32, "nla_put" },
	{ 0x852d599, "skb_push" },
	{ 0xbaf96c9c, "seq_release_net" },
	{ 0x9b27e9b2, "netif_stacked_transfer_operstate" },
	{ 0x2469810f, "__rcu_read_unlock" },
	{ 0x14e3a12f, "init_net" },
	{ 0xe0bb341b, "rtnl_link_unregister" },
	{ 0xcde64679, "__dev_get_by_index" },
	{ 0xc6cbbc89, "capable" },
	{ 0x347013de, "nla_validate" },
	{ 0xfa1d060f, "kmem_cache_alloc" },
	{ 0x3cab00cf, "arp_find" },
	{ 0xd3e6f60d, "cpu_possible_mask" },
	{ 0x94a8bd48, "eth_header_parse" },
	{ 0x6b2dc060, "dump_stack" },
	{ 0xd044be70, "alloc_netdev_mqs" },
	{ 0x72bf4213, "register_pernet_subsys" },
	{ 0x5eb8275a, "ether_setup" },
	{ 0x76d7c41e, "dev_uc_unsync" },
	{ 0x28cfe1ff, "__dev_get_by_name" },
	{ 0x341dbfa3, "__per_cpu_offset" },
	{ 0xa02f554f, "unregister_netdevice_queue" },
	{ 0x5042f8e4, "proc_create_data" },
	{ 0xed4a68d, "eth_validate_addr" },
	{ 0xb7d9ae9b, "seq_lseek" },
	{ 0x16112621, "dev_set_promiscuity" },
	{ 0x37a0cba, "kfree" },
	{ 0x9d669763, "memcpy" },
	{ 0xea093181, "seq_open_net" },
	{ 0x424a5e3f, "rtnl_link_register" },
	{ 0xadcf7d03, "dev_uc_del" },
	{ 0xce407bb2, "dev_uc_sync" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xb81960ca, "snprintf" },
	{ 0xab6c988b, "netdev_update_features" },
	{ 0x85670f1d, "rtnl_is_locked" },
	{ 0x7adf7bc9, "dev_queue_xmit" },
	{ 0x8d522714, "__rcu_read_lock" },
	{ 0xfd785250, "dev_mc_sync" },
	{ 0x6e720ff2, "rtnl_unlock" },
	{ 0xdf1e7dd7, "dev_get_stats" },
	{ 0xa7a17c91, "dev_set_mtu" },
	{ 0xe914e41e, "strcpy" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "87F39D15DFAE4DB2599E76D");
