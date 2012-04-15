#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <generated/compile.h>
#include <generated/utsrelease.h>
#include <linux/uts.h>
#include <linux/utsname.h>


ssize_t version_misc_read(struct file *filp, char __user * buf, size_t count,
			  loff_t * f_pos)
{
	char version_for_sys[] =
	    "Linux version " UTS_RELEASE " \n             (" LINUX_COMPILE_BY
	    "@" LINUX_COMPILE_HOST ") \n             (" LINUX_COMPILER
	    ") \n             " UTS_VERSION "\n\0";

	pr_info("%s +++\n%s", __func__, version_for_sys);

	if (copy_to_user(buf, version_for_sys, sizeof(version_for_sys)))
		return -EFAULT;

	return sizeof(version_for_sys);
}

struct file_operations misc_version_fops = {
	.owner = THIS_MODULE,
	.read = &version_misc_read,
};

struct miscdevice misc_version = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "misc_version_boot",
	.fops = &misc_version_fops,
};

static int __init msm_version_init(void)
{
	misc_register(&misc_version);
	return 0;
}

static void __exit msm_version_exit(void)
{
	misc_deregister(&misc_version);
}

module_init(msm_version_init);
module_exit(msm_version_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("ian sim");
MODULE_DESCRIPTION("version driver ");
MODULE_VERSION("1.0");
MODULE_ALIAS("platform:version");
