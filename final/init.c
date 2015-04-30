//*************************************************************************
//                      Logic of init.c 
// NOTE: this init.c creates only ONE login process on console=/dev/tty0
// YOUR init.c must also create login processes on serial ports /dev/ttyS0
// and /dev/ttyS1.. 
//************************************************************************

int pid, child, status;
int stdin, stdout;
int s0pid, s1pid;

#include "ucode.c"  
//#include "login.c"

main(int argc, char *argv[])
{
  /*1. // open /dev/tty0 as 0 (READ) and 1 (WRTIE) in order to display messages

  2. // Now we can use printf, which calls putc(), which writes to stdout*/
    close(0);
    close(1);

    stdin = open("/dev/tty0", O_RDONLY);
    stdout = open("/dev/tty0", O_WRONLY);

 
     printf("FSINIT: fork a login task on console\n"); 
     child = fork();

     if (child)
         parent();
     else  /* login task */
         login();
}       

int login()
{
  printf("entered login\n");
  exec("login /dev/tty0");
}
      
int parent()
{
  // Set up serial ports
  s0pid = fork();
  if(s0pid){
    // Parent
    s1pid = fork();
    if(s1pid){
      return;
    }else{
      // Child
       close(0);
    close(1);
    stdin = open("/dev/ttyS1", O_RDONLY);
    stdout = open("/dev/ttyS1", O_WRONLY);
    printf("entered parent\n");
    exec("login /dev/ttyS1");
    }
  }else{
    // Child, returning for now.
    //return;
    close(0);
    close(1);
    stdin = open("/dev/ttyS0", O_RDONLY);
    stdout = open("/dev/ttyS0", O_WRONLY);
    printf("entered child\n");
    exec("login /dev/ttyS0");
    
  }

  while(1){
    printf("FSINIT : waiting .....\n");
   // printf("before");
    pid = wait(&status);
    //printf("after");

    if (pid == child){
        //fork another login child
       printf("fork another login child");
        child = fork();
        if(child){
          parent();
        }else{
          login();
        }
    }else{
        printf("FSINIT : buried an orphan child %d\n", pid);
  }

}

}
