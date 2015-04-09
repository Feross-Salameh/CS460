int ksleep(int event)
{
	running->event = event;
	running->status = SLEEP;
	enqueue(&sleepList, running);
	tswitch();
}
  
int wakeup(int event)
{
	int i;
	for(i = 0; i < NPROC; i++)
	{
		if(proc[i].status == SLEEP && proc[i].event == event)
		{
			rmProc(&sleepList, &proc[i]);
			enqueue(&readyQueue, &proc[i]);
		}
	}
}

int kwait(int *status)
{
	int i, b = 0;
	for(i = 0; i < NPROC; i++)
		if(proc[i].ppid == running ->pid)
			b = 1;

	if(!b)
		return -1;
	while(1)
	{
		for(i = 0; i < NPROC; i++)
		{
			if(proc[i].status == ZOMBIE)
			{
				*status = proc[i].exitCode;
				b = proc[i].pid;
				proc[i].status = FREE;
				enqueue(&freeList, &proc[i]);
				return b;
			}
		}
		ksleep(running);
	}
}




int kexit(int exitValue)
{
	int i = 0;
	if(running->pid == 1)
	{
		for(i = 0; i < NPROC; i++)
		{
			if(i != 1 && proc[i].status == ZOMBIE)
			{
				printf("PROC 1 cannot die\n");
				return;
			}
		}
	}
	for(i = 0; i < NPROC; i++)
		if(proc[NPROC].ppid == 1)
		{
			wakeup(proc[1]);
			proc[NPROC].parent = &proc[1];
		}
	
	
	running->exitCode = exitValue;
	wakeup(running->parent);
	running->status = ZOMBIE;

	// ADD: close opened file descriptors
	for (i=0; i<NFD; i++){
		if (running->fd[i] != 0)
			close_pipe(i);
	}
	tswitch();
}

