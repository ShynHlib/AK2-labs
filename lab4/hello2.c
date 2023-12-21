#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <hello1.h>

static unsigned int n = 1;

MODULE_AUTHOR("Hlib Shyn <gshin907@gmail.com>");
MODULE_DESCRIPTION("Second module, prints 'Hello, world!'");
MODULE_LICENSE("Dual BSD/GPL");
module_param(n, uint, 0644);
MODULE_PARM_DESC(n, "Number of times to print 'Hello, world!'");

static int __init hello_init(void)
{
 int count;
 count = print_hello(n);

 return count;
}

static void __exit hello_exit(void)
{
 //
}

module_init(hello_init);
module_exit(hello_exit);

