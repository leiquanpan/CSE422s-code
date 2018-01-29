/*s program will create some number of seconds of work on each processor
 * on the system.
 *
 * This program requires the use of the OpenMP compiler flag, and that 
 * optimizations are turned off, to build correctly. E.g.: 
 * gcc -fopenmp workload.c -o workload
 */

#include <stdio.h> // for printf()
#include <sched.h> // for sched_getcpu()
#include <errno.h>
#include <stdbool.h>
// 500 million iterations should take several seconds to run
#define ITERS 500000000
#define LockState 0
#define UnlockState 1

#define FALSE 0
#define TRUE 1
volatile int type=UnlockState;

void critical_section( void ){
	int index = 0;
	while(index < ITERS){ index++; }
	printf("CPU %d finished in one critical_section!\n", sched_getcpu());
	
}

int lock_func(volatile int *type){
	int expected = UnlockState;
	int desired = LockState;
        while(!__atomic_compare_exchange(type, &expected, &desired, FALSE, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE)){
                expected = UnlockState;
        }

        return 1;
}


int unlock_func(volatile int *type){
	int expected = LockState;
	int desired = UnlockState;
        if(!__atomic_compare_exchange( type, &expected, &desired, FALSE, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE)){
                printf("Error: lock is not locked");
		expected = LockState;
        }

        return 1;
} 

int main (int argc, char* argv[]){

	// Create a team of threads on each processor
	#pragma omp parallel
	{
		// Each thread executes this code block independently

		lock_func(&type);
		critical_section();	
		unlock_func(&type);
		printf("CPU %d finished!\n", sched_getcpu());
	}

	return 0;
}
