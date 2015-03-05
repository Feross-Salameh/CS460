#include "type.h"

PROC proc[NPROC], *running, *freeList, *readyQueue, *sleepList;
int procSize = sizeof(PROC);
int nproc = 0;
int color;

int body();
int goUmode();
PROC *kufork(char *filename);
char *pname[]={"Sun", "Mercury", "Venus", "Earth",  "Mars", "Jupiter", 
               "Saturn", "Uranus", "Neptune" };
//#include "queue.c"

#include "int.c"

/***********************************************************
  kfork() creates a child proc and returns the child pid.
  When scheduled to run, the child process resumes to body();
************************************************************/
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

     // YOU WRITE CODE TO FILL IN uDS, uES, uCS

     /* initial USP relative to USS */
     p->usp = -2*12; 
     p->uss = segment;
  }

  printf("Proc %d kforked a child %d at segment=%x\n",
          running->pid, p->pid, segment);
  return p;
}

copy_image(u16 child_segment)
{
	u32 count = 0;
	u16 parent_segment = running->usp, temp = 0;
	printf("copy_image() called\n");
	for(count = 0; count < 32768; count += 2)
	{
		temp =(u16)get_word(parent_segment, count);
		put_word(temp, child_segment, count);
	}

}

PROC *kufork(char *filename)
{
	PROC *p;
	int  i, child;
	u16  child_segment;
	printf("NEW fork called\n");
	/*** get a PROC for child process: ***/
	if ( (p = get_proc(&freeList)) == 0)
	{
		printf("no more proc\n");
		return(-1);
	}
	/* initialize the new proc and its stack */
	p->status = READY;
	p->ppid = running->pid;
	p->parent = running;
	p->priority  = 1;
	// clear all SAVed registers on kstack
	for (i=1; i<10; i++)
		p->kstack[SSIZE-i] = 0;
	// fill in resume address
	p->kstack[SSIZE-1] = (int)goUmode;
	// save stack TOP address in PROC
	p->ksp = &(p->kstack[SSIZE - 9]);
	enqueue(&readyQueue, p);
	nproc++;
	if(filename)
	{
		child_segment = 0x1000*(p->pid+1);  // new PROC's segment
		load(filename, child_segment);      // load file to LOW END of segment	
		copy_image(child_segment);
		p->uss = child_segment;
		p->usp = running->usp;
		
		//for (i=1; i<=12; i++)
		//{         // write 0's to ALL of them
			//put_word(0, child_segment, -2*i);
		//}
     
		
		put_word(0x0200,   child_segment, -2*1);   /* flag */
		put_word(child_segment,  child_segment, -2*2);   /* uCS */  
		put_word(child_segment,  child_segment, -2*11);  /* uES */  
		put_word(child_segment,  child_segment, -2*12);  /* uDS */  

	}  
	
	printf("Proc %d forked a child %d at segment=%x\n",
		running->pid, p->pid, child_segment);
	return p;
}

int init()
{
    PROC *p; int i;
    color = 0x0C;
    printf("init ....");
    for (i=0; i<NPROC; i++){   // initialize all procs
        p = &proc[i];
        p->pid = i;
        p->status = FREE;
        p->priority = 0;  
        strcpy(proc[i].name, pname[i]);
   
        p->next = &proc[i+1];
    }     
    freeList = &proc[0];      // all procs are in freeList
    proc[NPROC-1].next = 0;
    readyQueue = sleepList = 0;

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
    if (running->status == READY)
        enqueue(&readyQueue, running);
     running = dequeue(&readyQueue);
     color = running->pid + 0x0A;
}

int int80h();

int set_vec(u16 vector, u16 handler)
{
     // put_word(word, segment, offset) in mtxlib
     put_word(handler, 0, vector<<2);
     put_word(0x1000,  0, (vector<<2) + 2);
}
            
main()
{
    printf("MTX starts in main()\n");
    init();      // initialize and create P0 as running
    set_vec(80, int80h);

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

