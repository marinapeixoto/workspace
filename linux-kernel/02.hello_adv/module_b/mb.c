#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

extern void hello_world(void);

static int __init mb_init(void) {
    printk(KERN_ALERT "mb init\n");
    hello_world();
    return 0;
}

static void __exit mb_exit(void) {
    printk(KERN_ALERT "mb exit\n");
}



module_init(mb_init);
module_exit(mb_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("tkorays");

