#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/errno.h>

static unsigned int n = 1;

MODULE_AUTHOR("Hlib Shyn <gshin907@gmail.com>");
MODULE_DESCRIPTION("Prints 'Hello world!'");
MODULE_LICENSE("Dual BSD/GPL");
module_param(n, uint, 0644);
MODULE_PARM_DESC(n, "Number of iterations to print 'hello'");

struct event_node {
    struct list_head list;
    ktime_t event_time;
};

static LIST_HEAD(event_list);

static int __init hello_init(void)
{
    if (n == 0 || (n >= 5 && n <= 10)) {
        pr_warn("Warning: the number is 0 or between 5 and 10\n");
    }
    else if (n > 10) {
        pr_err("Error: the number is greater that 10\n");
        return -EINVAL;
    }

    int i;
    for (i = 0; i < n; i++) {
        struct event_node* event = kmalloc(sizeof(*event), GFP_KERNEL);
        event->event_time = ktime_get();
        list_add(&event->list, &event_list);

        printk(KERN_EMERG "Hello, world!\n");
    }

    return 0;
}

static void __exit hello_exit(void)
{
    struct event_node* temp, * next;

    list_for_each_entry_safe(temp, next, &event_list, list) {
        ktime_t elapsed_time = ktime_sub(ktime_get(), temp->event_time);
        pr_info("Time: %lld ns\n", ktime_to_ns(elapsed_time));

        list_del(&temp->list);
        kfree(temp);
    }
}

module_init(hello_init);
module_exit(hello_exit);

