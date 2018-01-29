#ifndef __HEADER_H
#define __HEADER_H
#define MAX_SIZE 1000000
struct shared_data{

	volatile int write_guard;
	volatile int read_guard;
	volatile int delete_guard;
	volatile int data[MAX_SIZE];
};

const char* mRegion = "NIHAO";

#endif


