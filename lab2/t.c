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
	struct proccess *next;
	int    ksp;

	int    status;       // FREE|READY|SLEEP|BLOCK|ZOMBIE
	int    priority;     // priority
	int    pid;          // process pid
	int    ppid;         // parent pid 
	struct proccess *parent; // pointer to parent PROC

	int    kstack[SSIZE]; // SSIZE=1024
}PROC;


// #include "io.c" /**** USE YOUR OWN io.c with YOUR printf() here *****/

PROC proc[NPROC], *running, *freeList, *readyQueue;



int  procSize = sizeof(PROC);

/****************************************************************
 Initialize the proc's as shown:

 running->proc[0]--> proc[1] --> proc[2] ... --> proc[NPROC-1] -->
                       ^                                         |
            |<---------------------------------------<------------

 Each proc's kstack contains:
      retPC, ax, bx, cx, dx, bp, si, di, flag;  all 2 bytes
*****************************************************************/

// part 4.
enqueue(PROC **queue, PROC *p)
{
	
	if(!*queue)
	{
		*queue = p;
		return; 
	}
	// search for correct place...
	while(!(*queue)->next)
	{
		if(p->priority >= (*queue)->priority)
			break;
		else
		{
			p->next = *queue;
			queue = &p;
			return;
		}
	}
	
	// end of queue...
	(*queue)->next = p;
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
	printf("[%d, %d]", temp->pid, temp->priority);
	temp = temp->next;
	while(!temp)
	{
		printf("->[%d, %d]", temp->pid, temp->priority);
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
	
	p->status = READY;
	p->priority = 1;
	p->ppid = running->pid;
	p->parent = running;
	
	return p;
}


int body();  

int initialize()
{
  int i, j;
  PROC *p;

  for (i=0; i < NPROC; i++)
  {
    p = &proc[i];
    p->next = &proc[i+1];
    p->pid = i;
    p->status = FREE;
    p->priority = 0;
    p->ppid = 0;
    p->parent = 0;
    
    
    if (i)
    {     // initialize kstack[ ] of proc[1] to proc[N-1]
      for (j=1; j < 10; j++)
          p->kstack[SSIZE - j] = 0;          // all saved registers = 0
      p->kstack[SSIZE-1]=(int)body;          // called tswitch() from body
      p->ksp = &(p->kstack[SSIZE-9]);        // ksp -> kstack top
    }
    
  }
  running = proc;
  running->status = READY;
  running->parent = &proc[0];
  proc[NPROC -1].next = &proc[0];
  printf("initialization complete\n"); 
}

int body()
{
   char c;
   printf("proc % resumes to body() function\n");
   while(1)
   face{
      printf("I am Proc %d in body(): CMD[s|q|f]:  ", running->pid);
      c=getc();
      printf("%c\n", c);
      tswitch();
   }
}

main()
{
   char c;
   printf("\nWelcome to the 460 Multitasking System\n");
   initialize();
   while(1)
   {
     printf("proc %d running : enter a key : ", running->pid);
     c = getc();
     printf("%c\n", c); 
     tswitch();
   }
}

int scheduler()
{
    running = running->next;
}

