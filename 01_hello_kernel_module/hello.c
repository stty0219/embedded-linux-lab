#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

static int __init hello_init(void) {
    pr_info("Hello! My First Kernel Module!\n");
    return 0;
}

static void __exit hello_exit(void) {
    pr_info("Goodbye Kernel!\n");
}

module_init(hello_init);
module_exit(hello_exit);
