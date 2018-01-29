#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <linux/kthread.h>
#include <linux/sched.h>

/* init function - logs that initialization happened, returns success */
static unsigned long log_sec = 1;
static unsigned long log_nsec = 0;

//timer part
static ktime_t ktimes;
static struct hrtimer hrtimers;

//task part
static struct task_struct *task_pointer;


module_param(log_sec, ulong, 0000);
module_param(log_nsec, ulong, 0000);

static int thread_func(void *data)
{
	while(1)
	{
		printk(KERN_INFO"Another iteration of the loop has started,the value of nvscw: %d , the value of nivcsw: %d\n ",current->nvcsw,current->nivcsw);
		set_current_state(TASK_INTERRUPTIBLE);
		schedule();
		if (kthread_should_stop())
		{
			printk(KERN_INFO"The thread is terminating");
			return 0;
		}
	}
	return 0;
}

enum hrtimer_restart timer_callback(struct hrtimer *timer_for_restart)
{
	ktime_t currtime;
	currtime = ktime_get();
	ktimes = ktime_set( log_sec, log_nsec );
	hrtimer_forward( timer_for_restart, currtime , ktimes );
	wake_up_process(task_pointer);
	return HRTIMER_RESTART;
}


static int simple_init (void) {
    //timer part
    ktime_t kt;
    //task part
    task_pointer = kthread_run( thread_func,"data for thread_func","mythread");	
    //time part
    kt = ktime_set( log_sec, log_nsec); 
    hrtimer_init( &hrtimers, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
    hrtimers.function = &timer_callback;
    hrtimer_start( &hrtimers, kt, HRTIMER_MODE_REL );
    
    printk(KERN_ALERT "simple module initialized\n");
    return 0;
}

/* exit function - logs that the module is being removed */
static void simple_exit (void) {
    //time part
    hrtimer_cancel( &hrtimers );

    kthread_stop( task_pointer );
    printk(KERN_ALERT "simple module is being unloaded\n");
}

module_init (simple_init);
module_exit (simple_exit);

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("Chris Gill");
MODULE_DESCRIPTION ("Simple CSE 422 Module Template");
