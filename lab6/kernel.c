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
	PROC *p = get_proc();
	if(!p)
		return 0;
	p->status = READY;
	p->priority = 1;
	p->ppid = running->pid;
	p->parent = running;
	enqueue(&readyQueue, p);
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

