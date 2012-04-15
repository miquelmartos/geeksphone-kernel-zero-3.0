#ifdef CONFIG_BOARD_PW28

#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <generated/compile.h>
#include <generated/utsrelease.h>
#include <linux/uts.h>
#include <linux/utsname.h>
#include <linux/semaphore.h>
#include <mach/oem_rapi_client.h>

ssize_t rpc_client_misc_read(struct file *filp, char __user * buf, size_t count,
			     loff_t * f_pos)
{
	return 0;
}

struct file_operations misc_rpc_client_fops = {
	.owner = THIS_MODULE,
	.read = &rpc_client_misc_read,
};

struct miscdevice misc_rpc_client = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "misc_rpc_client",
	.fops = &misc_rpc_client_fops,
};

static int g_client_id[OEM_RAPI_CLIENT_EVENT_MAX] = { 0, 0, 0, 0, 0, };

static struct msm_rpc_client *g_client;
static DEFINE_SEMAPHORE(g_client_mutex);

int oem_rpc_client_register(int id)
{
	if (id == OEM_RAPI_CLIENT_EVENT_NONE
	    || id > (OEM_RAPI_CLIENT_EVENT_MAX - 1))
		return 0;
	if (g_client_id[id] == 0) {
		g_client_id[id] = id;
		return id;
	}
	return 0;
}

EXPORT_SYMBOL(oem_rpc_client_register);

void set_data_to_arm9(int id, char *in, int insize)
{
	struct oem_rapi_client_streaming_func_arg arg;
	struct oem_rapi_client_streaming_func_ret ret;
	int ret_val = 0;

	if (id == OEM_RAPI_CLIENT_EVENT_NONE
	    || id > (OEM_RAPI_CLIENT_EVENT_MAX - 1) || in == NULL
	    || insize > 128 || insize == 0)
		return;
	if (g_client_id[id] == 0) {
		return;
	}

	do {

		arg.cb_func = NULL;
		arg.handle = (void *)0;
		arg.in_len = insize;
		arg.input = in;
		arg.out_len_valid = 0;
		arg.output_valid = 0;
		arg.output_size = 0;
		arg.event = id;

		ret.out_len = NULL;
		ret.output = NULL;
		down(&g_client_mutex);
		ret_val =
		    oem_rapi_client_streaming_function(g_client, &arg, &ret);
		up(&g_client_mutex);

	} while (0);
}

EXPORT_SYMBOL(set_data_to_arm9);

static int __init msm_rpc_client_init(void)
{
	g_client = oem_rapi_client_init();
	misc_register(&misc_rpc_client);
	return 0;
}

static void __exit msm_rpc_client_exit(void)
{
	oem_rapi_client_close();
	misc_deregister(&misc_rpc_client);
}

module_init(msm_rpc_client_init);
module_exit(msm_rpc_client_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("ian sim");
MODULE_DESCRIPTION("version driver ");
MODULE_VERSION("1.0");
MODULE_ALIAS("platform:version");

#endif // CONFIG_BOARD_PW28

