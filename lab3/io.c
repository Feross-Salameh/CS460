#include "header.h"
#include <stdlib.h>



do_kfork()
{
	PROC *p;
	printf("Forking child process\n");
	p = kfork();
	if(p == 0)
	{
		printf("kfork() was unsuccessful\n");
	}
	else
	{
		printf("kfork() was successful\n");
		printf("returning child process %d\n", p->pid);
	}
	
}


do_tswitch()
{
	printf("switching to different proccess\n");
	tswitch();
	printf("ReadyQueue: "); printQueue(&readyQueue);
}

do_exit()
{
	int num = 0;
	char inp[8];
	printf("Exit Value: ");
	gets(inp);
	printf("\n\r");
	num = atoi(inp);
	printf("%d will be given to %d\n", num, running->pid);
	kexit(num);
	
}

do_sleep()
{
	int num = 0;
	char inp[8];
	printf("Event Value: ");
	gets(inp);
	printf("\n\r");
	num = atoi(inp);
	printf("%d will be given to %d\n", num, running->pid);
	ksleep(num);
}

do_wake()
{
	int num = 0;
	char inp[8];
	printf("Event Value: ");
	gets(inp);
	printf("\n\r");
	num = atoi(inp);
	printf("%d will be given to %d\n", num, running->pid);
	kwake(num);
}

do_wait()
{
	//int num = 0;
	//char inp[8];
	printf("wait entered");
	//gets(inp);
	//printf("\n\r");
	//num = atoi(inp);
	//printf("%d will be given to %d\n", num, running->pid);
	kwait(running->pid);
}

printLists()
{
	printf("-------------------------Process'--------------------------\n");
	printf("FreeList: "); printQueue(freeList);
	printf("ReadyQueue: ");printQueue(readyQueue);
	printf("-----------------------------------------------------------\n");
}

