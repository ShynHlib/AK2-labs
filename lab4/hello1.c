#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/errno.h>
#include <hello1.h>

MODULE_AUTHOR("Hlib Shyn <gshin907@gmail.com>");
MODULE_DESCRIPTION("First module, works with the list.");
MODULE_LICENSE("Dual BSD/GPL");

struct event_node {
 struct list_head list;
 ktime_t event_time;
};

static LIST_HEAD(head);

int print_hello(int n)
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
  struct event_node *event = kmalloc(sizeof(*event), GFP_KERNEL);

  event->event_time = ktime_get();

  pr_info("Hello, world!\n");

  list_add(&event->list, &head);
 }

 return 0;
}

EXPORT_SYMBOL(print_hello);

static int __init hello_init(void)
{
 return 0;
}

static void __exit hello_exit(void)
{
 struct event_node *temp, *next;

 list_for_each_entry_safe(temp, next, &head, list) {
  ktime_t elapsed_time = ktime_sub(ktime_get(), temp->event_time);

  pr_info("Time: %lld ns\n", ktime_to_ns(elapsed_time));

  list_del(&temp->list);
  kfree(temp);
 }
}

module_init(hello_init);
module_exit(hello_exit);

