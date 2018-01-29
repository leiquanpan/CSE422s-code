#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include "header.h"
#include <time.h>

int main(int arg, char **argv)
{
    int i;
    time_t t;
    int fd_shm;
    int data[MAX_SIZE];
    struct shared_data *shared_mem_ptr;
    srand((unsigned) time(&t));
    for(i = 0; i<MAX_SIZE; i++){
        data[i] = rand()%100;
        
    }
    
    fd_shm = shm_open(mRegion, O_RDWR | O_CREAT, S_IRWXU);
    if(fd_shm == -1){
        error("shm_open");
    }
    if (ftruncate (fd_shm, sizeof (struct shared_data)) == -1)
       error ("ftruncate");
    
    shared_mem_ptr= mmap (NULL, sizeof (struct shared_data), PROT_READ | PROT_WRITE, MAP_SHARED,fd_shm, 0);
    if ( shared_mem_ptr == MAP_FAILED)
       error ("mmap");
    while(shared_mem_ptr->write_guard == 0){}
    memcpy(shared_mem_ptr->data, data, sizeof(data));
    shared_mem_ptr->read_guard = 1;
    //for(i = 0; i<MAX_SIZE; i++){
    //    printf("data list is of %d element is: %d for local\n", i, data[i]);
    //}
    while(shared_mem_ptr->delete_guard == 0){}
    shm_unlink(mRegion);
    
    
}
