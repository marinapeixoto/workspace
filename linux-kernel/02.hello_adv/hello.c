#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/export.h>


static int __init hello_init(void) {
    printk(KERN_ALERT "hello init\n");
    return 0;
}

static void __exit hello_exit(void) {
    printk(KERN_ALERT "hello exit\n");
}

extern void hello_world(void) {
    printk(KERN_ALERT "hello!!\n");
}


module_init(hello_init);
module_exit(hello_exit);


EXPORT_SYMBOL(hello_world);

