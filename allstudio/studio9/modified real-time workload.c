#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
	int num_cpus = atoi(argv[1]);
	int i = 0;
	int k = 1;
	struct sched_param param;
	cpu_set_t my_set;
	CPU_ZERO(&my_set);
	CPU_SET(num_cpus,&my_set);
	sched_setaffinity(0, sizeof(cpu_set_t), &my_set);
	param.sched_priority = atoi(argv[2]);
	if(param.sched_priority > sched_get_priority_max(SCHED_RR)||param.sched_priority < sched_get_priority_min(SCHED_RR)){
		printf("error");
	}
	
	if(sched_setscheduler(0, SCHED_RR, &param)!=0){
	 perror("sched error");
	}
	while(k<atoi(argv[3])){
	fork();
	k++;
	}
	while(i<500000000){
	i += 1;
	}
	return 0;
}


