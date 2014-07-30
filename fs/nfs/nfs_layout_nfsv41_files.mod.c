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
	{ 0xc7f9e15b, "pnfs_unregister_layoutdriver" },
	{ 0xa890214e, "nfs4_schedule_session_recovery" },
	{ 0xd6bcfb3a, "nfs4_set_ds_client" },
	{ 0x1ab647de, "nfs_request_add_commit_list" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0xcf0d943c, "put_lseg" },
	{ 0x5fc56a46, "_raw_spin_unlock" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x97255bdf, "strlen" },
	{ 0x1960a27b, "page_address" },
	{ 0x99709ad0, "nfs_init_cinfo" },
	{ 0x10785a91, "malloc_sizes" },
	{ 0x11d8eaaa, "nfs_initiate_commit" },
	{ 0x2a3aa678, "_test_and_clear_bit" },
	{ 0xce7163fd, "nfs_commitdata_alloc" },
	{ 0xb6a812f6, "nfs4_proc_getdeviceinfo" },
	{ 0x649ecf20, "__alloc_pages_nodemask" },
	{ 0xa237947, "nfs41_setup_sequence" },
	{ 0x28195756, "pnfs_generic_pg_test" },
	{ 0xc8cbf708, "nfs4_schedule_lease_recovery" },
	{ 0x8f21df75, "pnfs_update_layout" },
	{ 0x1bde646a, "rpc_exit" },
	{ 0xd4d7a32, "_atomic_dec_and_lock" },
	{ 0xb3ed622e, "rpc_delay" },
	{ 0xcf960c75, "xdr_init_decode_pages" },
	{ 0x8fcad12b, "rpc_call_start" },
	{ 0x27e1a049, "printk" },
	{ 0x20c55ae0, "sscanf" },
	{ 0x5da84e86, "_pnfs_return_layout" },
	{ 0x8b838fff, "nfs_request_remove_commit_list" },
	{ 0x12406f4b, "nfs4_find_get_deviceid" },
	{ 0x84b183ae, "strncmp" },
	{ 0xf6a52405, "nfs_generic_pg_test" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0x758be841, "nfs_pageio_reset_write_mds" },
	{ 0x71e60a9b, "nfs_retry_commit" },
	{ 0x9173e25d, "nfs_initiate_write" },
	{ 0xd923aef9, "pnfs_destroy_layout" },
	{ 0x2d879b44, "contig_page_data" },
	{ 0xbeb7088d, "nfs4_put_deviceid_node" },
	{ 0x59e5070d, "__do_div64" },
	{ 0x27d120e6, "nfs_put_client" },
	{ 0xc426e19d, "nfs4_insert_deviceid_node" },
	{ 0x505a0a70, "pnfs_set_layoutcommit" },
	{ 0x617a218d, "__cond_resched_lock" },
	{ 0xc53fb96b, "nfs4_init_ds_session" },
	{ 0x9f984513, "strrchr" },
	{ 0xfa1d060f, "kmem_cache_alloc" },
	{ 0x40da4b76, "__free_pages" },
	{ 0x9ca970ad, "nfs_initiate_read" },
	{ 0xe7429549, "nfs4_schedule_stateid_recovery" },
	{ 0x6350ff71, "rpc_sleep_on" },
	{ 0xde48527e, "pnfs_generic_pg_readpages" },
	{ 0x9c0bd51f, "_raw_spin_lock" },
	{ 0xb48cb5, "pnfs_read_done_resend_to_mds" },
	{ 0x4eb68809, "rpc_count_iostats" },
	{ 0x5a2ba1cf, "rpc_restart_call_prepare" },
	{ 0x43c9b3df, "rpc_wake_up_queued_task" },
	{ 0xb659706e, "rpc_pton" },
	{ 0x9e3cab0d, "pnfs_generic_pg_writepages" },
	{ 0x37a0cba, "kfree" },
	{ 0x9d669763, "memcpy" },
	{ 0xef7f9e13, "pnfs_write_done_resend_to_mds" },
	{ 0x9f0243d6, "rpc_wake_up" },
	{ 0x300bd2ab, "nfs_commitdata_release" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x795cef61, "nfs_init_commit" },
	{ 0x676bbc0f, "_set_bit" },
	{ 0x81a6c8e1, "xdr_inline_decode" },
	{ 0xb81960ca, "snprintf" },
	{ 0xca54fee, "_test_and_set_bit" },
	{ 0x766ee12, "nfs_pageio_reset_read_mds" },
	{ 0xacf8e5ca, "nfs4_init_deviceid_node" },
	{ 0x49ebacbd, "_clear_bit" },
	{ 0x6d044c26, "param_ops_uint" },
	{ 0xe49647e4, "nfs_remove_bad_delegation" },
	{ 0x87fb535d, "xdr_set_scratch_buffer" },
	{ 0xd542439, "__ipv6_addr_type" },
	{ 0x8723f5c7, "pnfs_register_layoutdriver" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

