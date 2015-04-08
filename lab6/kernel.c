

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


//kexit(int value)
//{
	//int i = 0;
	//if(running->pid == 1)
	//{
		//for(i = 0; i < NPROC; i++)
		//{
			//if(i != 1 && proc[i].status == ZOMBIE)
			//{
				//printf("PROC 1 cannot die\n");
				//return;
			//}
		//}
	//}
	//for(i = 0; i < NPROC; i++)
		//if(proc[NPROC].ppid == 1)
		//{
			//kwake(proc[1]);
			//proc[NPROC].parent = &proc[1];
		//}
	
	
	//running->exitCode = value;
	//kwake(running->parent);
	//running->status = ZOMBIE;
	//tswitch();
//}


//ksleep(int value)
//{
	//running->event = value;
	//running->status = SLEEP;
	//enqueue(&sleepList, running);
	//tswitch();
//}

kwake(int value)
{
	int i;
	for(i = 0; i < NPROC; i++)
	{
		if(proc[i].status == SLEEP && proc[i].event == value)
		{
			rmProc(&sleepList, &proc[i]);
			enqueue(&readyQueue, &proc[i]);
		}
	}
}

//kwait(int *status)
//{
	//int i, b = 0;
	//for(i = 0; i < NPROC; i++)
		//if(proc[i].ppid == running ->pid)
			//b = 1;
			
	//if(!b)
	//return -1;
	
	//while(1)
	//{
		//for(i = 0; i < NPROC; i++)
		//{
			//if(proc[i].status == ZOMBIE)
			//{
				//*status = proc[i].exitCode;
				//b = proc[i].pid;
				//proc[i].status = FREE;
				//enqueue(&freeList, &proc[i]);
				//return b;
			//}
		//}
		//ksleep(running);
	//}
//}
