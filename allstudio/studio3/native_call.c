#define _GNU_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
void main(){
	printf ("syscall to getuid returned: %u\n", syscall(__NR_getuid)); 
	int return_val;
	return_val = syscall(__NR_setuid);
	if( return_val != 0 ){ 
		printf("Error: setuid failed! Reason: %s\n", strerror(errno));
	}
}
