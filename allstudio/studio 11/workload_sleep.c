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
#include <sys/syscall.h>
#include <linux/futex.h>
#include <sys/time.h>
#include <limits.h>
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
	int ret_val;
	ret_val = __atomic_sub_fetch(type,1,__ATOMIC_ACQ_REL);
	while(ret_val<0){
		__atomic_store_n( type, -1, __ATOMIC_RELEASE );
		syscall( SYS_futex, type, FUTEX_WAIT, -1, NULL );
		ret_val = __atomic_sub_fetch(type,1,__ATOMIC_ACQ_REL);
	}

        return 1;
}


int unlock_func(volatile int *type){
	int ret_val;
	ret_val = __atomic_add_fetch( type, 1, __ATOMIC_ACQ_REL );
        if(ret_val != 1){
		__atomic_store_n( type, 1, __ATOMIC_RELEASE );
		syscall( SYS_futex, type, FUTEX_WAKE, INT_MAX );
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
