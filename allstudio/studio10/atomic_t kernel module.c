#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/types.h>
#include<linux/time.h>
#define iters 1000000

/* init function - logs that initialization happened, returns success */
//static unsigned long log_sec = 1;
//static unsigned long log_nsec = 0;

/*
//timer part
static ktime_t ktimes;
static struct hrtimer hrtimers;
*/
atomic_t share_data;
int count = 0;
//task part
static struct task_struct *task_pointer_00;
static struct task_struct *task_pointer_01;
static struct task_struct *task_pointer_02;
static struct task_struct *task_pointer_03;

/*
module_param(log_sec, ulong, 0644);
module_param(log_nsec, ulong, 0644);
*/

static int thread_func(void *data)
{	
	//srandom32((unit)jiffies);
	//time_t x = time(0);
	int x = count++;
	printk(KERN_INFO"share_data at the beginning in %d: %d",x,atomic_read(&share_data));
        int i = 0;
	for (i=0; i<iters;i++){
		atomic_add(1,&share_data);
	
	}
	printk(KERN_INFO"share data at the end in %d: %d",x,atomic_read(&share_data));
	return 0;
}
/*
enum hrtimer_restart timer_callback(struct hrtimer *timer_for_restart)
{
	ktime_t currtime;
	currtime = ktime_get();
	ktimes = ktime_set( log_sec, log_nsec );
	hrtimer_forward( timer_for_restart, currtime , ktimes );
	wake_up_process(task_pointer_00);
	wake_up_process(task_pointer_01);
	wake_up_process(task_pointer_02);
	wake_up_process(task_pointer_03);	
	return HRTIMER_RESTART;
}
*/

static int simple_init (void) {
/*    //timer part
    ktime_t kt;
*/
    printk(KERN_INFO "begin");
    atomic_set(&share_data,0);
    printk(KERN_INFO "The begin shared_data is: %d", atomic_read(&share_data));
    //task part
    task_pointer_00 = kthread_create( thread_func,"data for thread_func","mythread for core 0");
    kthread_bind(task_pointer_00,0);
    task_pointer_01 = kthread_create( thread_func,"data for thread_func","mythread for core 1");
    kthread_bind(task_pointer_01,1);
    task_pointer_02 = kthread_create( thread_func,"data for thread_func","mythread for core 2");
    kthread_bind(task_pointer_02,2);
    task_pointer_03 = kthread_create( thread_func,"data for thread_func","mythread for core 3");	
    kthread_bind(task_pointer_03,3);
/*    //time part
    kt = ktime_set( log_sec, log_nsec); 
    hrtimer_init( &hrtimers, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
    hrtimers.function = &timer_callback;
    hrtimer_start( &hrtimers, kt, HRTIMER_MODE_REL );
*/  
    wake_up_process(task_pointer_00);
    wake_up_process(task_pointer_01);
    wake_up_process(task_pointer_02);
    wake_up_process(task_pointer_03);
    printk(KERN_ALERT "simple module initialized\n");
    return 0;
}

/* exit function - logs that the module is being removed */
static void simple_exit (void) {
/*    //time part
    hrtimer_cancel( &hrtimers );
*/
    /*
    if(task_pointer_00){
        kthread_stop( task_pointer_00 );
    }
    if(task_pointer_01){
    kthread_stop( task_pointer_01 );
    }
    if(task_pointer_02){
    kthread_stop( task_pointer_02 );
    }
    if(task_pointer_03){
    kthread_stop( task_pointer_03 );
    }
*/
    printk(KERN_INFO "The shared_data is: %d", atomic_read(&share_data));
    printk(KERN_ALERT "simple module is being unloaded\n");
}

module_init (simple_init);
module_exit (simple_exit);

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("Chris Gill");
MODULE_DESCRIPTION ("Simple CSE 422 Module Template");
