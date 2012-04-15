/*
 * battery_charger_usb.c
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/fs.h>

#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>

/*-------------------------------------------------------------------------*/

#define DRIVER_DESC	"charger Gadget"
#define DRIVER_VERSION	"2010 10 21"

static const char shortname[] = "charger";
static const char driver_desc[] = DRIVER_DESC;

#ifdef CONFIG_USB_GADGET_DUALSPEED
#define DEVSPEED	USB_SPEED_HIGH
#else   /* full speed (low speed doesn't do bulk) */
#define DEVSPEED        USB_SPEED_FULL
#endif

/*-------------------------------------------------------------------------*/

static int charger_bind(struct usb_gadget *gadget)
{
	pr_info("%s +++\n", __func__);
	pr_info("%s ---\n", __func__);
	return 0;
}

static void charger_unbind(struct usb_gadget *gadget)
{
	pr_info("%s +++\n", __func__);
	set_gadget_data(gadget, NULL);
	pr_info("%s ---\n", __func__);
}

static int charger_setup(struct usb_gadget *gadget, const struct usb_ctrlrequest *ctrl)
{
	pr_info("%s +++\n", __func__);
	pr_info("%s ---\n", __func__);
	return 0;
}

static void charger_disconnect(struct usb_gadget *gadget)
{
	pr_info("%s +++\n", __func__);
	pr_info("%s ---\n", __func__);
}


static void charger_suspend(struct usb_gadget *gadget)
{
	pr_info("%s +++\n", __func__);
	pr_info("%s ---\n", __func__);
}

static void charger_resume(struct usb_gadget *gadget)
{
	pr_info("%s +++\n", __func__);
	pr_info("%s ---\n", __func__);
}

/*-------------------------------------------------------------------------*/

static struct usb_gadget_driver charger_driver = {
	.function	= (char *)driver_desc,
//	.bind		= charger_bind,
	.unbind		= charger_unbind,
	.setup		= charger_setup,
	.disconnect	= charger_disconnect,
	.suspend	= charger_suspend,
	.resume		= charger_resume,
	.driver		= {
		.name	= (char *)shortname,
		.owner	= THIS_MODULE,
	},
};

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_AUTHOR("Craig Nadler");
MODULE_LICENSE("GPL");

extern int battchg_pause;

static int __init init(void)
{
	int status = 0;

	pr_info("%s +++\n", __func__);
	if(battchg_pause == 1)
	status = usb_gadget_probe_driver(&charger_driver, charger_bind);
	pr_info("%s ---\n", __func__);
	return status;
}

module_init(init);

int register_charger_usb_init(void)
{
	int status;

	pr_info("%s +++\n", __func__);
        status = usb_gadget_probe_driver(&charger_driver, charger_bind);
	pr_info("%s ---\n", __func__);
	return status;
}
EXPORT_SYMBOL(register_charger_usb_init);

void unregister_charger_usb(void)
{
	int status;

	pr_info("%s +++\n", __func__);
        status = usb_gadget_probe_driver(&charger_driver, charger_bind);
	if (status)
		pr_err("%s: %x\n", __func__, status);
	pr_info("%s ---\n", __func__);
}
EXPORT_SYMBOL(unregister_charger_usb);

static void __exit cleanup(void)
{
	int status;

        status = usb_gadget_probe_driver(&charger_driver, charger_bind);
	if (status)
		pr_err("%s: usb_gadget_unregister_driver %x\n", __func__, status);
}

module_exit(cleanup);

