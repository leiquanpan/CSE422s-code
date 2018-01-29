/*************
 * 
 * leiquanpan
 * tree_fork.c
 * studio6
 * ***********/

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc,char* argv[])
{
	int generations;
	int cur_gene=1;
	pid_t pid;

	if(argc!=2)
	{
		printf("Usage: ./tree_fork <number of generations>");
		exit(-1);
	}
	
	generations=atoi(argv[1]);

	if(generations>10||generations==0)
	{
		printf("Please input number less than 10, greater than 0");
		exit(-1);
	}
	
	while(generations--)
	{
		pid=fork();
		if(pid>0)
		{
			if(cur_gene==1)
			{
				printf("I belong to generation %d, my pid is %d\n",cur_gene++,getpid());

                        	if(generations==0)
                                	return 0;
			}
	
		}
		else if(pid==0)
		{
			
			printf("I belong to generation %d, my pid is %d\n",cur_gene++,getpid());    

                        if(generations==0)
                                return 0;
                
                  	wait(0); 
		}
		else 
		{
			printf("Error\n");
		}
	}
	return 1;
}
