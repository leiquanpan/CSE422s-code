/******
*
* Leiquan Pan
* stdio6---simple_fork.c
*********/

#include <unistd.h>
#include <stdio.h>

int main(int argc,char* argv[])
{
	pid_t pid;
	pid=fork();
	if(pid>0)
	{
		printf("This is parent process,my pid is %d, the children pid is %d\n",getpid(),pid);
	}
	else if (pid==0)
	{
		printf("This is children process,my pid is %d, my parents' pid is %ld\n",getpid(),(long)getppid());	
	}
	else
		printf("Fork error\n");
	
	return 1;
}

