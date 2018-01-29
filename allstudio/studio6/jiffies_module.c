
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

/* init function - logs that initialization happened, returns success */
static int simple_init (void) {
    printk(KERN_ALERT "module initialized at %lu jiffies\n", jiffies);
    return 0;
}

/* exit function - logs that the module is being removed */
static void simple_exit (void) {
    printk(KERN_ALERT "module unloaded at %lu jiffies\n", jiffies);
}

module_init (simple_init);
module_exit (simple_exit);

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("Chris Gill");
MODULE_DESCRIPTION ("Simple CSE 422 Module Template");
