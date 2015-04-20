#include "type.h"

PROC proc[NPROC], *running, *freeList, *readyQueue, *sleepList;
int procSize = sizeof(PROC);
int nproc = 0;

int body();
int goUmode();
PROC *kfork(char *filename);
char *pname[]={"Sun", "Mercury", "Venus", "Earth",  "Mars", "Jupiter", 
               "Saturn", "Uranus", "Neptune" };
char *hh[ ] = {"FREE   ", "READY  ", "RUNNING", "STOPPED", "SLEEP  ", 
               "ZOMBIE ",  0}; 
OFT  oft[NOFT];
PIPE pipe[NPIPE];



/**************************************************
  bio.o, queue.o loader.o are in mtxlib
**************************************************/
//#include "wait.c"
//#include "kernel.c"
#include "int.c"
#include "forkexec.c"
#include "pipe.c"

PROC *kfork(char *filename)
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
	if (filename){

	 segment = 0x1000*(p->pid+1);  // new PROC's segment
	 load(filename, segment);      // load file to LOW END of segment

	 /********** ustack contents at HIGH END of ustack[ ] ************
		PROC.usp
	   -----|------------------------------------------------
		  |uDS|uES|udi|usi|ubp|udx|ucx|ubx|uax|uPC|uCS|flag|
	   -----------------------------------------------------
		   -12 -11 -10 -9  -8  -7  -6  -5  -4  -3  -2   -1
	 *****************************************************************/

	 for (i=1; i<=12; i++){         // write 0's to ALL of them
		 put_word(0, segment, -2*i);
	 }
	 
	 put_word(0x0200,   segment, -2*1);   /* flag */  
	 put_word(segment,  segment, -2*2);   /* uCS */  
	 put_word(segment,  segment, -2*11);  /* uES */  
	 put_word(segment,  segment, -2*12);  /* uDS */  

	 /* initial USP relative to USS */
	 p->usp = -2*12; 
	 p->uss = segment;
	}

	printf("Proc %d kforked a child %d at segment=%x\n",
		  running->pid, p->pid, segment);
	return p;
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

int do_ps()
{
   int i,j; 
   char *p, *q, buf[16];
   buf[15] = 0;

   printf("============================================\n");
   printf("  name         status      pid       ppid  \n");
   printf("--------------------------------------------\n");

   for (i=0; i<NPROC; i++){
       strcpy(buf,"               ");
       p = proc[i].name;
       j = 0;
       while (*p){
             buf[j] = *p; j++; p++;
       }      
       prints(buf);    prints(" ");
       
       if (proc[i].status != FREE){
           if (running==&proc[i])
              prints("running");
           else
              prints(hh[proc[i].status]);
           prints("     ");
           printd(proc[i].pid);  prints("        ");
           printd(proc[i].ppid);
       }
       else{
              prints("FREE");
       }
       printf("\n");
   }
   printf("---------------------------------------------\n");

   return(0);
}


int kmode()
{
	running->kstack[SSIZE-1] = (int)body;
	body();
}


#define NAMELEN 32
int chname(char * y)
{
  char buf[64];
  char *cp = buf;
  int count = 0; 

  while (count < NAMELEN){
     *cp = get_byte(running->uss, y);
     if (*cp == 0) break;
     cp++; y++; count++;
  }
  buf[31] = 0;

  printf("changing name of proc %d to %s\n", running->pid, buf);
  strcpy(running->name, buf); 
  printf("done\n");
}


int init()
{
    PROC *p;
    int i, j;
    printf("init ....");
    for (i=0; i<NPROC; i++){   // initialize all procs
        p = &proc[i];
        p->pid = i;
        p->status = FREE;
        p->priority = 0;  
        strcpy(proc[i].name, pname[i]);
        p->next = &proc[i+1];

        // clear fd[ ] array of PROC
        for (j=0; j<NFD; j++)
             p->fd[j] = 0;
    }
    freeList = &proc[0];      // all procs are in freeList
    proc[NPROC-1].next = 0;
    readyQueue = sleepList = 0;

    // initialize all OFT and PIPE structures    
    for (i=0; i<NOFT; i++)
        oft[i].refCount = 0;
    for (i=0; i<NPIPE; i++)
        pipe[i].busy = 0;

    /**** create P0 as running ******/
    p = get_proc(&freeList);
    p->status = READY;
    p->ppid   = 0;
    p->parent = p;
    running = p;
    nproc = 1;
    printf("done\n");
} 

int scheduler()
{
    if (running->status == READY){
       enqueue(&readyQueue, running);
    }
    printList("readyQueue", readyQueue);
    running = dequeue(&readyQueue);
}

int int80h();

int set_vector(u16 vector, u16 addr)
{
    u16 location,cs;
    location = vector << 2;
    put_word(addr, 0, location);
    put_word(0x1000,0,location+2);
}
            
main()
{
    printf("MTX starts in main()\n");
    init();      // initialize and create P0 as running
    set_vector(80,int80h);

    kfork("/bin/u1");     // P0 kfork() P1

    while(1){
      printf("P0 running\n");
      if (nproc==2 && proc[1].status != READY)
	  printf("no runable process, system halts\n");
      while(!readyQueue);
      printf("P0 switch process\n");
      tswitch();   // P0 switch to run P1
   }
}

printLists()
{
	printf("-------------------------Process'--------------------------\n");
	printf("FreeList: "); printQueue(freeList);
	printf("ReadyQueue: ");printQueue(readyQueue);
	printf("-----------------------------------------------------------\n");
}

do_goUmode()
{
	goUmode();
}
do_sleep()
{
	ksleep(running->pid);
}


do_wake()
{
	wakeup(running->ppid);
}

