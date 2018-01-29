//
//  main.c
//  lab2
//
//  Created by Leiquan Pan on 11/5/17.
//  Copyright © 2017 Leiquan Pan. All rights reserved.
//
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

volatile int data[81];
volatile int read_count = 0;
int data_num;
volatile int rounds;
volatile int iterations;
pthread_mutex_t lock_m;
pthread_spinlock_t lock_s;
char *wait_strategy;
char *scheduler;
pthread_barrier_t barr;

void *cube_function(void *ptr)
{
    //pthread barrier implemtation
    pthread_barrier_wait(&barr);
    int iter;
    int temp;
    //read the rounds value
    int round = rounds;
    
    //begin rounds
    while (round--)
    {
        iter = iterations;
        
        //lock to protect data
        if (!strcmp(wait_strategy, "spin")){
            pthread_spin_lock(&lock_s);
        }
        else
            pthread_mutex_lock(&lock_m);
        
        //critical section
        temp = data[read_count];
        if(read_count++ >= data_num-1)
           read_count = 0;
//        printf("The number is %d\n",temp);
        //unlock
        if (!strcmp(wait_strategy, "spin"))
        {
	    pthread_spin_unlock(&lock_s);
        }
        else
            pthread_mutex_unlock(&lock_m);
        while (iter--)
	{
		temp = temp*temp*temp;
//		printf("");
	}
//        printf("The number is %d\n",temp);
        //end one round
    }
    return 0;
}

int main(int argc, char * argv[]) {
    const char *message1 = "Thread 1";
    const char *message2 = "Thread 2";
    int thread_s2, thread_s1;
    int t_cpu_s1, t_cpu_s2;
    cpu_set_t cpuset;
    int core_num;
    int policy;
    int sched_state_1,sched_state_2;

    pthread_t thread[8];
    int i = 0;
    
    //check input and process input
    if(argc < 6)
    {
        printf("Usage <program_name> <scheduler> <wait-strategy> <rounds> <iterations> <number>+ ");
    }
    scheduler = argv[1];
    wait_strategy = argv[2];
    rounds = atoi(argv[3]);
    iterations = atoi(argv[4]);
    for(i = 5; i < argc ; i ++)
        data[i - 5] = atoi(argv[i]);
    data_num = argc - 5;
    if( (strcmp(scheduler,"SCHED_RR") && strcmp(scheduler,"SCHED_FIFO") && strcmp(scheduler,"SCHED_NORMAL")) || (strcmp(wait_strategy,"spin") && strcmp(wait_strategy,"sleep")))
    {
        printf("Invalid aruments\n");
        return -1;
    }
    
    // initialize lock
    pthread_mutex_init(&lock_m, NULL);
    pthread_spin_init(&lock_s, 0);
    pthread_barrier_init(&barr,NULL,8);

    //check the policy
    if (!strcmp(scheduler, "SCHED_RR"))
        policy = SCHED_RR;
    else if (!strcmp(scheduler, "SCHED_FIFO"))
        policy = SCHED_FIFO;
    else if (!strcmp(scheduler, "SCHED_NORMAL"))
        policy = SCHED_OTHER;
//    printf("The things is %s",scheduler);
    //get  proper priority
    int midpriority = (sched_get_priority_max(policy)+sched_get_priority_min(policy))/2;
    
    //create thread for four cores
    for (core_num = 0; core_num < 4; core_num++)
    {
        //set schedule param
        struct sched_param param;
        param.sched_priority = midpriority;
        
        //set cpu param
        CPU_ZERO(&cpuset);
        CPU_SET(core_num, &cpuset);
        
        // create two thread and sched them immediately
        thread_s1 = pthread_create( &thread[core_num*2], NULL, cube_function, (void*) message1);
        if(thread_s1)
        {
            fprintf(stderr,"Error - pthread_create(thread1) return code: %d\n",thread_s1);
            return -1;
	}
//     	printf("the value 1 is %d\n",param.sched_priority);
        sched_state_1 = pthread_setschedparam(thread[core_num*2],policy,&param);
        if(sched_state_1)
        {
            fprintf(stderr,"Error - pthread_setschedparam(thread1) return code: %d\n",sched_state_1);
        }
//	printf("the value 2 is %d\n",param.sched_priority);
	param.sched_priority = midpriority + 1;
        thread_s2 = pthread_create( &thread[core_num*2+1], NULL, cube_function, (void*) message2);
        if(thread_s2)
        {
            fprintf(stderr,"Error - pthread_create(thread2) return code: %d\n",thread_s2);
            return -1;
        }
        sched_state_2 = pthread_setschedparam(thread[core_num*2+1],policy,&param);
        if(sched_state_2)
        {
            fprintf(stderr,"Error - pthread_setschedparam(thread 2) return code: %d\n",sched_state_2);
        }
        
        // set affinity
        t_cpu_s1 = pthread_setaffinity_np(thread[core_num*2], sizeof(cpu_set_t), &cpuset);
        if (t_cpu_s1 != 0)
        {
            fprintf(stderr,"Error - pthread_setaffinity_np(thread1) return code: %d\n",t_cpu_s1);
        }
        t_cpu_s2 = pthread_setaffinity_np(thread[core_num*2+1], sizeof(cpu_set_t), &cpuset);
        if (t_cpu_s2 != 0)
        {
            fprintf(stderr,"Error - pthread_setaffinity_np(thread2) return code: %d\n",t_cpu_s2);
        }
        
        //set clear
        CPU_CLR(core_num,&cpuset);
    }

    for(i = 0; i < 8; i++)
    {
        pthread_join( thread[i],NULL);
    }
    //destroy lock
    pthread_mutex_destroy(&lock_m);
    pthread_spin_destroy(&lock_s);
    pthread_barrier_destroy(&barr);

    printf("Hello, World!\n");
    exit(EXIT_SUCCESS);
    return 0;
}

