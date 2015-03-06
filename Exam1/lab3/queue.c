#include "header.h"

enqueue(PROC **queue, PROC *p)
{
	if(*queue == 0)
	{
		*queue = p;
		return; 
	}
	
	while(1)
	{
		if((*queue)->priority <= p->priority)
		{
			(p->next) = *queue;
			*queue = p;
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
	*queue = ((*queue)->next);
	ret->next = 0;
	
	return ret;
	
}

printQueue(PROC *queue)
{
	PROC *temp = queue;
	if(queue == 0)
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


rmProc(PROC **list, PROC *p)
{
	PROC * temp;
	if((*list) == 0)
		return;
	
	if((*list)->event == p->event)
		(*list) = 0;
		
	while((*list)->next != 0)
	{
		temp = (*list)->next;
		if(temp->event == p->event)
			temp = temp->next;
		(*list) = (*list)->next;
	}
}

