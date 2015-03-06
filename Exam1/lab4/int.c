/*************************************************************************
  usp  1   2   3   4   5   6   7   8   9  10   11   12    13  14  15  16
----------------------------------------------------------------------------
 |uds|ues|udi|usi|ubp|udx|ucx|ubx|uax|upc|ucs|uflag|retPC| a | b | c | d |
----------------------------------------------------------------------------
***************************************************************************/
#define PA 9
#define PB 10
#define PC 11
#define PD 12
#define AX  4

print_stack(int length, u16 seg)
{
	int i;
	u16 word = 0;
	printf("printing stack at: %x, %d many times.\n", seg, length);
	for(i = 1; i < length; i++)
	{
		word = get_word(seg, -2*i);
		printf("%d: |%x|\n", -i, word);
		
	}
	printf("\nEnd of print stack\n");
	getc();
}



/****************** syscall handler in C ***************************/
int kcinth()
{
   u16    segment, offset;
   int    a,b,c,d, r;
   char inp(21);
   segment = running->uss; 
   offset = running->usp;
   /** get syscall parameters from ustack **/
   a = get_word(segment, offset + 2*PA);
   b = get_word(segment, offset + 2*PB);
   c = get_word(segment, offset + 2*PC);
   d = get_word(segment, offset + 2*PD);


   switch(a){
       case 0 : r = running->pid;     break;
       case 1 : r = kps();            break;
       case 2 : r = kchname(b);       break;
       case 3 : r = kkfork();         break;
       case 4 : r = ktswitch();       break;
       case 5 : r = kkwait(b);        break;
       case 6 : r = kkexit(b);        break;
       case 7 : r = khop(b);
       case 12 : r = kgetname(b);	  break;

   case 90: r = getc();               break;
   case 91: r = putc(b);              break;

   case 99: kkexit(b);                break;
       default: printf("invalid syscall # : %d\n", a); 
   }
   put_word(r, segment, offset + 2*AX);
}


//int color;
extern PROC proc[];

char *hh[ ] = {"FREE   ", "READY  ", "RUNNING", "STOPPED", "SLEEP  ", 
               "ZOMBIE ",  0}; 
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

#define NAMELEN 32
int kchname(char * y)
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

int kps()
{
  return do_ps();
}

int kgetname(char *name[64])
{
	int len = 0;
	printf("getting name...\n");
	len = getname(*name);
	printf("in int.c: %s, len %d\n", *name, len);
	//len = strlen(*name);
	return len;
}

int khop(u32 newsegment)
{
	printf("p1 attempting to hop to %x\n", newsegment);
	
	return hop(newsegment);
	
}

int kkfork()
{
  return do_kfork("/bin/u1");
}

int ktswitch()
{
   return tswitch();
}

