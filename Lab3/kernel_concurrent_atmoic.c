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
#include<linux/slab.h>

#define MAX_THREAD 100
/* init function - logs that initialization happened, returns success */
static unsigned long num_threads = 1;
static unsigned long upper_bound = 10;

//data_array
atomic_t *share_data;
int *counter_variables;
int counter = 0;

//pointers
int pointer_to_counterv;
atomic_t pointer_to_array;
int cur_prime_number;

//atomic
atomic_t atomic_variable;
atomic_t barrier_variable_first;
atomic_t barrier_variable_second;

//task part
static struct task_struct *task_pointers;

//global variables for time stamp
struct timeval start, first_barrier, last_barrier;

/*param input*/
module_param(num_threads, ulong, 0644);
module_param(upper_bound, ulong, 0644);

int Prime_function(int local_count_pos)
{
    int local_variables;
    int i;

    while(atomic_read(&pointer_to_array) < upper_bound - 1)
    {
        if(atomic_read(&pointer_to_array) > upper_bound - 2)
        {
            return 0;
        }
        local_variables = atomic_read(&share_data[atomic_read(&pointer_to_array)]);
        atomic_add(1,&pointer_to_array);
        
        if(!local_variables)
            continue;
        
        for(i = local_variables * 2; i <= upper_bound; i += local_variables )
        {
            atomic_set(&share_data[i - 2],0);
            counter_variables[local_count_pos]++;
        }
    }
    return 1;
}

static int thread_func(void *data)
{
    //get the counter variables location
    int local_count_pos = (int) data;
//    printk(KERN_ALERT "The current counter variable location is %d\n",local_count_pos);
    //barrier one
    atomic_add(1,&barrier_variable_first);
    if(atomic_read(&barrier_variable_first) == num_threads)
    {
        do_gettimeofday(&first_barrier);
        //printk(KERN_ALERT "The first barrier reach time is %lu seconds, %lu useconds \n",first_barrier.tv_sec, first_barrier.tv_usec);
    }
    while(atomic_read(&barrier_variable_first) < num_threads);
    
    //start prime
    Prime_function(local_count_pos);
    
    //barrier two
    atomic_add(1,&barrier_variable_second);
    if(atomic_read(&barrier_variable_second) == num_threads)
    {
        do_gettimeofday(&last_barrier);
        //printk(KERN_ALERT "The second barrier reach time is %lu seconds, %lu useconds \n",last_barrier.tv_sec, last_barrier.tv_usec);
    }
    while(atomic_read(&barrier_variable_second) < num_threads);
    
    //set atomic variable
    if(atomic_read(&atomic_variable) == 0)
        atomic_add(1,&atomic_variable);
    
    return 0;
}

static int simple_init (void) {
    
    int i = 2;
    //get the time stamp
    do_gettimeofday(&start);
    
    //check module_param's validity
    if(upper_bound < 2 || num_threads < 1)
    {
        printk(KERN_ALERT "Param initialization wrong, the upper_bound is %lu, the num_threads is %lu!\n",upper_bound,num_threads);
        return -1;
    }
    
    //allocate share_data memory and initialize them
    share_data = (atomic_t *)kmalloc((upper_bound - 1)*sizeof(atomic_t), GFP_KERNEL);
    for(;i <= upper_bound; i++)
    {
        atomic_set(&share_data[i - 2],i);
    }
    
    //initialization
    pointer_to_counterv = 0;
    cur_prime_number = 2;
    atomic_set(&pointer_to_array,0);
    
    //set atomic and print message
    atomic_set(&atomic_variable,0);
    atomic_set(&barrier_variable_first,0);
    atomic_set(&barrier_variable_second,0);
    
    if(atomic_read(&atomic_variable) == 0)
        printk(KERN_ALERT "All the kernel threads haven't finished yet!\n");
    
    
    //allocate memory for couter variables array and intialize
    i = num_threads;
    counter_variables = (int *)kmalloc(num_threads*sizeof(int), GFP_KERNEL);

    for(i = 0; i < num_threads; i++)
    {
	counter_variables[i] = 0;
    }

    //create num_threads threads and wake up them
    while(i--)
    {
        task_pointers = kthread_create(&thread_func,(void *)pointer_to_counterv,"mythread for core 0");
        pointer_to_counterv++;
        wake_up_process(task_pointers);
    }
    
    printk(KERN_ALERT "simple module initialized\n");
    return 0;
}

/* exit function - logs that the module is being removed */
static void simple_exit (void) {
    int i = 0;
    int cross_out = 0;
    unsigned long num = 0;
    if(atomic_read(&atomic_variable) == 0)
    {
        printk(KERN_INFO "The processing has not finished");
        return;
    }
    
    for(; i <= upper_bound - 2; i++)
    {
        if(atomic_read(&share_data[i]))
        {
            printk(KERN_ALERT "%d  ",atomic_read(&share_data[i]));
            num++;
            if(num % 8 == 0)
                printk(KERN_ALERT "\n");
        }
    }
    printk(KERN_ALERT "The number of prime number is %lu, the number of non-prime number is %lu!\n",num,upper_bound - 1 - num);
    printk(KERN_ALERT "The upper_bound is %lu, the num_threads is %lu!\n",upper_bound,num_threads);
    printk(KERN_ALERT "The initialization time is %lu seconds, %lu useconds \n",first_barrier.tv_sec - start.tv_sec, first_barrier.tv_usec - start.tv_usec);
    printk(KERN_ALERT "The processing time is %lu seconds, %lu useconds \n",last_barrier.tv_sec - first_barrier.tv_sec, last_barrier.tv_usec - first_barrier.tv_usec);

    for(i = 0; i < num_threads; i++)
    {
        cross_out += counter_variables[i];
    }
    printk(KERN_ALERT "The number of useless cross out is %d\n",cross_out - (int)upper_bound + 1 + (int)num);
    //free memory
    kfree(share_data);
    kfree(counter_variables);
    
    return;
}

module_init (simple_init);
module_exit (simple_exit);

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("Leiquan Pan");
MODULE_DESCRIPTION ("Kernel Sychronization");


