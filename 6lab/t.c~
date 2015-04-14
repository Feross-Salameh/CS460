#include "type.h"

PROC proc[NPROC], *running, *freeList, *readyQueue, *sleepList;
int procSize = sizeof(PROC);
int nproc = 0;

int body();
int goUmode();
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

    printf("enter a char [s|f|q|l|p|z|a|w|u] : ");
    c = getc(); printf("%c\n", c);
  
    switch(c){
       case 's' : do_tswitch();   break;
       case 'f' : do_kfork();     break;
       case 'q' : do_exit();      break;
       case 'l' : printLists();   break; 
       case 'p' : do_ps();        break; 
       case 'z' : do_sleep();     break; 
       case 'a' : do_wake();    break; 
       case 'w' : do_wait();      break;
       case 'u' : goUmode();		break;
     }
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
	running->kstack[SSIZE-1] = (int)goUmode;
	//goUmode();
}
do_sleep()
{
	ksleep(running->pid);
}

do_kfork()
{
	PROC *p;
	printf("Forking child process\n");
	p = kfork();
	if(p == 0)
	{
		printf("kfork() was unsuccessful\n");
	}
	else
	{
		printf("kfork() was successful\n");
		printf("returning child process %d\n", p->pid);
	}
	
}

do_wake()
{
	wakeup(running->pid);
}

do_tswitch()
{
	printf("switching to different proccess\n");
	tswitch();
	printf("ReadyQueue: "); printQueue(&readyQueue);
}
