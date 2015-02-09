/************ t.c file **********************************/
#define NPROC     9        
#define SSIZE  1024                /* kstack int size */

#define DEAD      0                /* proc status     */
#define READY     1      
#define FREE      2               
#define SLEEP     3 
#define BLOCK     4                
#define ZOMBIE    5                

typedef struct proccess
{
	struct proc *next;
	int    ksp;

	int    status;       // FREE|READY|SLEEP|BLOCK|ZOMBIE
	int    priority;     // priority
	int    pid;          // process pid
	int    ppid;         // parent pid 
	struct proc *parent; // pointer to parent PROC

	int    kstack[SSIZE]; // SSIZE=1024
}PROC;


// #include "io.c" /**** USE YOUR OWN io.c with YOUR printf() here *****/

PROC proc[NPROC], *running, *freeList, *readyQueue;



int  procSize = sizeof(PROC);

// part 4.
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



// part 3.
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

// part 5 
PROC *kfork()
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

kexit()
{
	printf("kexit(): entered \n");
	running->status = ZOMBIE;
	printf("kexit(): running is now zombie\n");
	tswitch();
	printf("kexit(): past tswitch\n");
}

printLists()
{
	printf("-------------------------Process'--------------------------\n");
	printf("FreeList: "); printQueue(freeList);
	printf("ReadyQueue: ");printQueue(readyQueue);
	printf("-----------------------------------------------------------\n");
}

int body();  

int initialize()
{
	int i, j;
	PROC *p, *temp;
	for (i=0; i < NPROC; i++)
	{
		p = &proc[i];
		p->next = 0;
		p->pid = i;
		p->status = FREE;
		p->priority = 0;
		p->ppid = 0;
		p->parent = 0;
		p->next = 0;
		if (i)
		{     // initialize kstack[ ] of proc[1] to proc[N-1]
			for (j=1; j < 10; j++)
				p->kstack[SSIZE - j] = 0;          // all saved registers = 0
			p->kstack[SSIZE-1]=(int)body;          // called tswitch() from body
			p->ksp = &(p->kstack[SSIZE-9]);        // ksp -> kstack top
		}
	}
	running = &proc[0];
	running->status = READY;
	running->parent = &proc[0];
	freeList = &proc[1];
	for(i = 2; i < NPROC; i++)
	{
		proc[i-1].next = &proc[i];
	}
	
	readyQueue = 0;
}

int body()
{
	char c;
	printf("proc % resumes to body() function\n");
	while(1)
	{
		printLists();
		printf("I am Proc %d in body(): CMD[s|q|f]:  ", running->pid);
		c=getc();
		printf("%c\n", c);
		switch(c)
		{
		case 's':
			printf("Calling tswitch\n");
			tswitch();
			break;
		case 'q':
			kexit();
			break;
		case 'f':
			if(kfork())
				printf("kfork was successful\n");
			else
				printf("kfork failed\n");
			break;
		default:
			printf("invalid command\n");
		}
	}
}

main()
{
   char c;
   printf("\nWelcome to the 460 Multitasking System\n");
   initialize();
   while(1)
	{
		printLists();
		printf("I am Proc %din main(), CMD[s|q|f]:  ", running->pid);
		c=getc();
		printf("%c\n", c);
		switch(c)
		{
		case 's':
			printf("Calling tswitch\n");
			tswitch();
			break;
		case 'q':
			kexit();
			break;
		case 'f':
			if(kfork())
				printf("kfork was successful\n");
			else
				printf("kfork failed\n");
			break;
		default:
			printf("invalid command\n");
		}
	}
}

int scheduler()
{
    if (running->status == READY)
          enqueue(&readyQueue, running);
       running = dequeue(&readyQueue);
}

