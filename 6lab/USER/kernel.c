

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



PROC *kfork()
{
	PROC *p;
	int  i, child;
	u16  segment;
	printf("kfork called\n");
	/*** get a PROC for child process: ***/
	if ( (p = get_proc(&freeList)) == 0){
	   printf("no more proc\n");
	   return(-1);
	}
	/* initialize the new proc and its stack */
	p->status = READY;
	p->ppid = running->pid;
	p->parent = running;
	p->priority  = 1;                 // all of the same priority 1

	// clear all SAVed registers on kstack
	for (i=1; i<10; i++)
	  p->kstack[SSIZE-i] = 0;

	// fill in resume address
	p->kstack[SSIZE-1] = (int)body;
	// save stack TOP address in PROC
	p->ksp = &(p->kstack[SSIZE - 9]);

	enqueue(&readyQueue, p);

	nproc++;
	return p;
}

rmProc(PROC **list, PROC *p)
{
	PROC *temp;
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
