/*
* hello.c – The Hello, World! Kernel Module
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include<linux/moduleparam.h>
#include<linux/hrtimer.h>
#include<linux/ktime.h>
/*
* hello_init – the init function, called when the module is loaded.
* Returns zero if successfully loaded, nonzero otherwise.
*/
static unsigned long log_sec=1;
static unsigned long log_nsec=0;

module_param(log_sec, ulong, 0000);
module_param(log_nsec, ulong, 0000);
static struct hrtimer timer;
static ktime_t ktime;
static enum hrtimer_restart callback(struct hrtimer *timer){
	ktime_t curtime;
	curtime = ktime_get();
	hrtimer_forward(timer,curtime,ktime);
	printk(KERN_INFO "krtimer restart.");
	return HRTIMER_RESTART;
}
static int hello_init(void)
{

ktime = ktime_set(log_sec, log_nsec);
hrtimer_init(&timer, CLOCK_MONOTONIC,HRTIMER_MODE_REL);
timer.function = &callback;
hrtimer_start( &timer, ktime, HRTIMER_MODE_REL );
printk(KERN_ALERT "simple module initialied");

return 0;
}
/*
* hello_exit – the exit function, called when the module is removed.

*/
static void hello_exit(void)
{
int cancel;
cancel = hrtimer_cancel(&timer);
if(cancel) printk(KERN_ALERT "timer still in use");
printk(KERN_ALERT "simple module unloaded.");
}
module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GRILL");
MODULE_DESCRIPTION("SIMPLE CSE422 EXAMPLE");
