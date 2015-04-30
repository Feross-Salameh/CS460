//***********************************************************************
//                   LOGIC of login.c file
//***********************************************************************

#include "ucode.c"

char *tty;

main(int argc, char *argv[])   // invoked by exec("login /dev/ttyxx")
{

  char user[32], pass[32]; // User input
  char * puser, *ppass, *gid, *uid, *fullname, *homedir, *program;
  int fd;
  int bytes;
  char buf[1024];
  char * pch;
  char * users[8];
  int i = 0, j = 0;
  int stdin, stdout, stderr;
  int numUsers;

  tty =  argv[1];

/* 1. close(0); close(1); close(2); // login process may run on different terms

 2. // open its own tty (passed in by INIT) as stdin, stdout, stderr

 3. settty(tty);   // store tty string in PROC.tty[] for putc() */
  close(0);
  close(1);
  close(2);

  stdin = open(tty, O_RDONLY);
  stdout = open(tty, O_WRONLY);
  stderr = open(tty, O_RDWR);

  settty(tty);

  // NOW we can use printf, which calls putc() to our tty
  printf("FSLOGIN : open %s as stdin, stdout, stderr\n", tty);

  signal(2,1);  // ignore Control-C interrupts so that 
                // Control-C KILLs other procs on this tty but not the main sh

  /*while(1){
    1. show login:           to stdout
    2. read user nmae        from stdin
    3. show passwd:
    4. read user passwd

    5. verify user name and passwd from /etc/passwd file

    6. if (user account valid){
          setuid to user uid.
          chdir to user HOME directory.
          exec to the program in users's account
       }*/

      while(1)
      {
         printf("login: ");
         gets(user);
         printf("password: ");
         gets(pass);

        fd = open("/etc/passwd", O_RDONLY);

         // Now get the lines in the form
         // username:password:gid:uid:fullname:HOMEDIR:program
         while(bytes = read(fd, buf, 256));

         pch = strtok(buf, "\n");
         while(pch != 0)
	 {
          users[i++] = pch;
          pch = strtok(0, "\n");
         }
         numUsers = i;
          // Now users[] contains all profiles
          i = 0;
          j = 0;
          while(i < numUsers)
	  {
                 // printf("users[%d] = %s\n", i, users[i]);
                  pch = strtok(users[i], ":");
                  j=0;
                  while(pch != 0)
		  {
                        switch(j)
			{
                          case 0:
                            puser = pch;
                            break;
                          case 1:
                            ppass = pch;
                            break;
                          case 2:
                            gid = pch;
                            break;
                          case 3:
                            uid = pch;
                            break;
                          case 4:
                            fullname = pch;
                            break;
                          case 5:
                            homedir = pch;
                            break;
                          case 6:
                            program = pch;
                            break;
                        }

                     

                      pch = strtok(0, ":");
                      j++;
                    } // End strtok while


                // Here check for login credentials
                   /* printf("-%s-%s-\n", user, puser);
                    printf("-%s-%s-\n", pass, ppass);
                    getc();*/
                if(!strcmp(user, puser)==0 && strcmp(pass, ppass))
		{
                  printf("LOGIN SUCCESSFUL\n\n");

                  printf("----- %s's Profile -----\n", puser);
                  printf("username: %s\n", puser);
                  printf("password: %s\n", ppass);
                  printf("uid: %d\n", (int)atoi(uid));
                  printf("gid: %d\n", (int)atoi(gid));
                  printf("homedir: %s\n", homedir);
                  printf("program: %s\n", program);

                  // Now set up UID and GID
                  chuid((int)atoi(uid), (int)atoi(gid));

                  // Set up home directory
                  chdir(homedir);

                  // Exec to program
                  exec(program);
                  break;
                }

                i++;
            } // End iteration while

            close(fd);
      } // End while(1)
       printf("login failed, try again\n");
      
  
}
