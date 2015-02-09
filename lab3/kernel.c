
PROC *get_proc()
{
	PROC * ret; 
	if(!freeList)
		return 0;
	ret = freeList;
	freeList = freeList->next;
	ret->next = 0;
	return ret; 
}
#include "header.h"

put_proc(PROC *p)
{
	PROC *ptr;
	if(!freeList)
	{
		freeList = p;
		return;
	}
	ptr = freeList;
	while(ptr->next)
		ptr = ptr->next;
	ptr->next = p;	
}

PROC *do_kfork()
{
	PROC *p = get_proc();
	if(!p)
		return 0;
	printf("kfork(): recieved process %d\n", p->pid);
	p->status = READY;
	p->priority = 1;
	p->ppid = running->pid;
	p->parent = running;
	enqueue(&readyQueue, p);
	printf("kfork(): readyQueue is now %d\n", readyQueue->pid);
	return p;
}
