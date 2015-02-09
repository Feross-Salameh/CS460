#include "header.h"

enqueue(PROC **queue, PROC *p)
{
	printf("enqueue(): Entered\n");
	if(*queue == 0)
	{
		printf("enqueue(): queue empty, p %d is now queue\n", p->pid);
		*queue = p;
		return; 
	}
	
	while(1)
	{
		printf("enqueue(): beginning of loop\n");
		if((*queue)->priority <= p->priority)
		{
			printf("enqueue(): inserting p %d\n", p->pid);
			(p->next) = *queue;
			*queue = p;
			printf("enqueue(): queue is now %d\n", (*queue)->pid);
			return;
		}
		if((*queue)->next == 0)
		{
			(*queue)->next = p;
			return;
		}
		(*queue)->next = (*queue);
	}

	return;
	
}
PROC *dequeue(PROC **queue)
{
	
	// need to grab first object into queue
	PROC *ret;
	
	ret = *queue;
	
	*queue = (*queue)->next;
	
	ret->next = 0;
	
	return ret;
	
}
printQueue(PROC *queue)
{
	PROC *temp = queue;
	if(!temp)
	{
		printf("Nothing in queue\n");
		return;
	}
	printf("[%d,%d]", temp->pid, temp->priority);
	temp = temp->next;
	while(temp != 0)
	{
		printf("->[%d,%d]", temp->pid, temp->priority);
		temp = temp->next;
	}
	printf("\n");
	
}
