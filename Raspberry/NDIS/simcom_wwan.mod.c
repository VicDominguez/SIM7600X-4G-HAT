#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x17300f3a, "module_layout" },
	{ 0x3f7955b, "usbnet_disconnect" },
	{ 0xfc1babcd, "usbnet_probe" },
	{ 0x2622982, "usb_deregister" },
	{ 0xe58a2083, "usb_register_driver" },
	{ 0xbcee12ad, "skb_push" },
	{ 0x4054d820, "__dev_kfree_skb_any" },
	{ 0x1b27718b, "_dev_err" },
	{ 0x15df7be, "skb_pull" },
	{ 0xfab94f80, "usbnet_suspend" },
	{ 0x76d2a466, "usbnet_resume" },
	{ 0x7ca8488c, "usb_control_msg" },
	{ 0x7c32d0f0, "printk" },
	{ 0xef022990, "_dev_info" },
	{ 0xf4ccd685, "usbnet_get_endpoints" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("usb:v1E0Ep9025d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1E0Ep9001d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "ACED7068FFE31726F8F05EE");
