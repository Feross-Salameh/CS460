#include "header.h"
#include "kernel.c"
#include "queue.c"
#include "io.c"

int body();  

int init()
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

int scheduler()
{
  if (running->status == READY)
      enqueue(&readyQueue, running);
  running = dequeue(&readyQueue);
  color = 0x000A + (running->pid % 6);
}
            
main()
{
    printf("MTX starts in main()\n");
    init();      // initialize and create P0 as running
    kfork();     // P0 kfork() P1
    while(1){
      printf("P0 running\n");
      if (nproc==2 && proc[1].status != READY)
	  printf("no runable process, system halts\n");
      while(!readyQueue);
      printf("P0 switch process\n");
      tswitch();   // P0 switch to run P1
   }
}

int body()
{
  char c;
  printf("proc %d resumes to body()\n", running->pid);
  while(1){
    printf("-----------------------------------------\n");
            //print freeList;
            printf("freeList: "); printQueue(freeList);
            // print readyQueue;
            printf("readyQueue: "); printQueue(readyQueue);
            // print sleepList;
            printf("sleepList: "); printQueue(sleepList);
    printf("-----------------------------------------\n");

    printf("proc %d[%d] running: parent=%d\n",
	   running->pid, running->priority, running->ppid);

    printf("enter a char [s|f|q|l|p|z|a|w] : ");
    c = getc(); printf("%c\n", c);
  
    switch(c){
       case 's' : do_tswitch();   break;
       case 'f' : do_kfork();     break;
       case 'q' : do_exit();      break;
       case 'l' : printLists();   break; 
       //case 'p' : do_ps();        break; 
       case 'z' : do_sleep();     break; 
       case 'a' : do_wake();    break; 
       case 'w' : do_wait();      break;
     }
  }
}

/***********************************************************
  Write YOUR C code for
        ksleep(), kwakeup()
        kexit()
        kwait()

Then, write your C code for
      do_ps(), do_sleep(), do_wakeup(), do_wait()
************************************************************/
