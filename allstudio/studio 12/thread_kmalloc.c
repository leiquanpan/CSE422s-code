#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/time.h>
/* init function - logs that initialization happened, returns success */
static unsigned long param = 2000;
//timer part
//task part
static struct task_struct *task_pointer;

typedef struct intergers
{
    unsigned int array[8];
}Intergers;

module_param(param, ulong, 0000);

static int thread_func(void *data)
{
    struct intergers *kmalloc_mem;
    struct intergers *p;
    int number = 0;
    int result = 1;
    struct timespec tp_start;
    struct timespec tp_end;
    while(result < param)
    {
        number++;
        result = result*2;
    }
    getrawmonotonic(&tp_start);
    while(param--)
    {
        kmalloc_mem = (struct intergers *)kmalloc(sizeof(struct intergers), GFP_KERNEL);
        kfree(kmalloc_mem);
    }
    getrawmonotonic(&tp_end);
    printk(KERN_INFO "The interval sencond is %d,the interval msecond is %d",tp_end.tv_sec-tp_start.tv_sec,tp_end.tv_nsec-tp_start.tv_nsec);
	return 0;
}

static int simple_init (void) {
    //task part
    task_pointer = kthread_create( thread_func,"data for thread_func","mythread for core 0");
    kthread_bind(task_pointer,0);
    //time part
    wake_up_process(task_pointer);
    printk(KERN_ALERT "simple module initialized\n");
    return 0;
}

/* exit function - logs that the module is being removed */
static void simple_exit (void) {
    //time part
    kthread_stop( task_pointer );
    printk(KERN_ALERT "simple module is being unloaded\n");
}

module_init (simple_init);
module_exit (simple_exit);

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("Chris Gill");
MODULE_DESCRIPTION ("Simple CSE 422 Module Template");
