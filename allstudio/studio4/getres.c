/**************
*getres.c
*This program output the resolution for different clock types.
*************/

#include<time.h>
#include<stdio.h>

int main(int argc,char* argv[])
{
	struct timespec tp;
	int i;

	clockid_t clock_types[]={CLOCK_REALTIME,CLOCK_MONOTONIC,CLOCK_MONOTONIC_COARSE,CLOCK_PROCESS_CPUTIME_ID,CLOCK_THREAD_CPUTIME_ID};
	
	for(i=0;i<5;i++)
	{
		//read successfully
		if(!clock_getres(clock_types[i],&tp))
			printf("Clock %d, Seconds: %ld, Nanoseconds: %ld\n",i,tp.tv_sec,tp.tv_nsec);
		else
			printf("Clock %d not supported\n",i);
	}	
	
	return 1;
}

