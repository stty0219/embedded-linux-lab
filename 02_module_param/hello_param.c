#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

static char *name = "World";
static int count = 1;

module_param(name, charp, 0644);
MODULE_PARM_DESC(name, "Who to greet (default: World)");

module_param(count, int, 0644);
MODULE_PARM_DESC(count, "How many times to greet (default: 1)");

static int __init hello_param_init(void)
{
	int i;

	for (i = 0; i < count; i++)
		pr_info("hello_param: Hello, %s! (%d/%d)\n", name, i + 1, count);
	return 0;
}

static void __exit hello_param_exit(void)
{
	pr_info("hello_param: Goodbye, %s!\n", name);
}

module_init(hello_param_init);
module_exit(hello_param_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module parameter demo");