#define _GNU_SOURCE
#define __NR_noargs 391
#define __NR_oneargs 392
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
void main(){
	printf ("syscall to getuid returned: %u\n", syscall(__NR_noargs)); 
	int return_val;
	return_val = syscall(__NR_oneargs);
	if( return_val != 0 ){ 
		printf("Error: setuid failed! Reason: %s\n", strerror(errno));
	}
}
