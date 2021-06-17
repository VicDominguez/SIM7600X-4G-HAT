/*
 * Copyright (c) 2016 Xiaobin Wang <xiaobin.wang@sim.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 */

/*
 * history 
 * V1.00 - first release  -20160822
*/

#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/ethtool.h>
#include <linux/etherdevice.h>
#include <linux/mii.h>
#include <linux/usb.h>
#include <linux/usb/cdc.h>
#include <linux/usb/usbnet.h>


/* very simplistic detection of IPv4 or IPv6 headers */
static bool possibly_iphdr(const char *data)
{
	return (data[0] & 0xd0) == 0x40;
}

/* SIMCOM devices combine the "control" and "data" functions into a
 * single interface with all three endpoints: interrupt + bulk in and
 * out
 */
static int simcom_wwan_bind(struct usbnet *dev, struct usb_interface *intf)
{
	int rv = -EINVAL;
  
	//struct usb_driver *subdriver = NULL;
	atomic_t *pmcount = (void *)&dev->data[1];

  /* ignore any interface with additional descriptors */
	if (intf->cur_altsetting->extralen)
		goto err;
  
	/* Some makes devices where the interface descriptors and endpoint
	 * configurations of two or more interfaces are identical, even
	 * though the functions are completely different.  If set, then
	 * driver_info->data is a bitmap of acceptable interface numbers
	 * allowing us to bind to one such interface without binding to
	 * all of them
	 */
	if (dev->driver_info->data &&
	    !test_bit(intf->cur_altsetting->desc.bInterfaceNumber, &dev->driver_info->data)) {
		dev_info(&intf->dev, "not on our whitelist - ignored");
		rv = -ENODEV;
		goto err;
	}

	atomic_set(pmcount, 0);

	/* collect all three endpoints */
	rv = usbnet_get_endpoints(dev, intf);
	if (rv < 0)
		goto err;

	/* require interrupt endpoint for subdriver */
	if (!dev->status) {
		rv = -EINVAL;
		goto err;
	}

	/* can't let usbnet use the interrupt endpoint */
	dev->status = NULL;

	printk("simcom usbnet bind here\n");

  /*
   * SIMCOM SIM7600 only support the RAW_IP mode, so the host net driver would
   * remove the arp so the packets can transmit to the modem
  */
  dev->net->flags |= IFF_NOARP;
    
  /* make MAC addr easily distinguishable from an IP header */
	if (possibly_iphdr(dev->net->dev_addr)) {
		dev->net->dev_addr[0] |= 0x02;	/* set local assignment bit */
		dev->net->dev_addr[0] &= 0xbf;	/* clear "IP" bit */
	}
	
  /*
   * SIMCOM SIM7600 need set line state
  */
  usb_control_msg(
      interface_to_usbdev(intf),
      usb_sndctrlpipe(interface_to_usbdev(intf), 0),
      0x22, //USB_CDC_REQ_SET_CONTROL_LINE_STATE
      0x21, //USB_DIR_OUT | USB_TYPE_CLASS| USB_RECIP_INTERFACE
      1, //line state 1
      intf->cur_altsetting->desc.bInterfaceNumber,
      NULL,0,100);

err:
	return rv;
}

static void simcom_wwan_unbind(struct usbnet *dev, struct usb_interface *intf)
{
	struct usb_driver *subdriver = (void *)dev->data[0];

	if (subdriver && subdriver->disconnect)
		subdriver->disconnect(intf);

	dev->data[0] = (unsigned long)NULL;
}

#ifdef CONFIG_PM
static int simcom_wwan_suspend(struct usb_interface *intf, pm_message_t message)
{
	struct usbnet *dev = usb_get_intfdata(intf);
	int ret;

	ret = usbnet_suspend(intf, message);
	if (ret < 0)
		goto err;
		
err:
	return ret;
}

static int simcom_wwan_resume(struct usb_interface *intf)
{
	struct usbnet *dev = usb_get_intfdata(intf);
	int ret = 0;
	
	ret = usbnet_resume(intf);

err:
	return ret;
}
#endif

struct sk_buff *simcom_wwan_tx_fixup(struct usbnet *dev, struct sk_buff *skb, gfp_t flags)
{

  //skip ethernet header 
  if(skb_pull(skb, ETH_HLEN))
  {
    return skb;
  }else
  {
    dev_err(&dev->intf->dev, "Packet Dropped\n");
  }

  if (skb != NULL)
    dev_kfree_skb_any(skb);

   return NULL;
}

static int simcom_wwan_rx_fixup(struct usbnet *dev, struct sk_buff *skb)
{
  __be16 proto;

  /* This check is no longer done by usbnet */
  if (skb->len < dev->net->hard_header_len)
    return 0;

	switch (skb->data[0] & 0xf0) {
	case 0x40:
		printk("packetv4 coming ,,,\n");
		proto = htons(ETH_P_IP);
		break;
	case 0x60:
		printk("packetv6 coming ,,,\n");
		proto = htons(ETH_P_IPV6);
		break;
	case 0x00:
		printk("packet coming ,,,\n");
		if (is_multicast_ether_addr(skb->data))
			return 1;
		/* possibly bogus destination - rewrite just in case */
		skb_reset_mac_header(skb);
		goto fix_dest;
	default:
		/* pass along other packets without modifications */
		return 1;
	}
	if (skb_headroom(skb) < ETH_HLEN)
		return 0;
	skb_push(skb, ETH_HLEN);
	skb_reset_mac_header(skb);
	eth_hdr(skb)->h_proto = proto;
	memset(eth_hdr(skb)->h_source, 0, ETH_ALEN);
fix_dest:
	memcpy(eth_hdr(skb)->h_dest, dev->net->dev_addr, ETH_ALEN);
	return 1;
}

static const struct driver_info	simcom_wwan_usbnet_driver_info = {
	.description	= "SIMCOM wwan/QMI device",
	.flags		= FLAG_WWAN,
	.bind		  = simcom_wwan_bind,
	.unbind		= simcom_wwan_unbind,
	.rx_fixup       = simcom_wwan_rx_fixup,
	.tx_fixup       = simcom_wwan_tx_fixup,
};

static const struct usb_device_id products[] = {
  {USB_DEVICE(0x1e0e, 0x9025), .driver_info = (unsigned long)&simcom_wwan_usbnet_driver_info },
	{USB_DEVICE(0x1e0e, 0x9001), .driver_info = (unsigned long)&simcom_wwan_usbnet_driver_info },
	{ } /* END */
};

MODULE_DEVICE_TABLE(usb, products);

static struct usb_driver simcom_wwan_usb_driver = {
	.name		        = "simcom_wwan",
	.id_table	      = products,
	.probe		      =	usbnet_probe,
	.disconnect	    = usbnet_disconnect,
#ifdef CONFIG_PM
	.suspend	      = simcom_wwan_suspend,
	.resume		      =	simcom_wwan_resume,
	.reset_resume         = simcom_wwan_resume,
	.supports_autosuspend = 1,
#endif
};

static int __init simcom_wwan_init(void)
{
	return usb_register(&simcom_wwan_usb_driver);
}
module_init(simcom_wwan_init);

static void __exit simcom_wwan_exit(void)
{
	usb_deregister(&simcom_wwan_usb_driver);
}
module_exit(simcom_wwan_exit);

MODULE_AUTHOR("Xiaobin Wang <xiaobin.wang@sim.com>");
MODULE_DESCRIPTION("SIM7600 RMNET WWAN driver");
MODULE_LICENSE("GPL");
