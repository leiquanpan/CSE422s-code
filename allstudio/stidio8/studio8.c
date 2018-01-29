#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
	int num_cpus = atoi(argv[1]);
	
		cpu_set_t my_set;
		CPU_ZERO(&my_set);
		CPU_SET(num_cpus,&my_set);
		sched_setaffinity(0, sizeof(cpu_set_t), &my_set);
	while(1);
}


