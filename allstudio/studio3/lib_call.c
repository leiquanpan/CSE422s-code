#define _GNU_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
void main(){
	printf ("getuid returned: %u\n", getuid()); 
	int return_val;
	return_val = setuid(0);
	if( return_val != 0 ){ 
		printf("Error: setuid failed! Reason: %s\n", strerror(errno));
	}
}
