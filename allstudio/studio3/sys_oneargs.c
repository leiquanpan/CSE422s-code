/* This file implements a demonstration syscall for an OS course. It
* takes no arguments and prints out a simple message to the kernel
*  log, indicating that it was called.
*  */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>

//Define a system call implementation that takes no arguments
SYSCALL_DEFINE1( oneargs, int, param_name ){

  // print out a simple message indicating the function was called, and return SUCCESS
    printk("Someone invoked the sys_oneargs system call, param_name:%u\n",param_name);
      return 0;  
      }

      //End of file
