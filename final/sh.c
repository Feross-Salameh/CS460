/*
	sh.c shell program
*/
#define FILEOUT 1
#define FILEIN 2
#define FILEAPPEND 3

#include "ucode.c"

char cmd[256];
char *pipeCmds[16];
int numCmds;
int pd[2];


int main(int argc, char *argv[]) {
	 int pid, i = 0;

	while(1){
		printf("fsh:# ");
		gets(cmd);

		if(!strcmp(cmd, "logout") || !strcmp(cmd, "exit") ){
			//logout
			exit(&cmd);
		}

		// Check how many pipes are needed
		numCmds = getPipeCmds();
		if(numCmds == 1){
				
				pid = fork();
				if(pid==0){
					manageFileIO(pipeCmds[0]);
					//printf("pipeCmds: %s", pipeCmds[0]);
					//getc();
					exec(pipeCmds[0]);
				}else{
					pid = wait(getpid());
				}

		}else if(numCmds > 1){
			// We must pipe each one
			pid = fork();
			if(pid == 0){
				recursivePipe(pipeCmds[numCmds-2], numCmds-1);
				exec(pipeCmds[numCmds-1]);

			}else{
				pid = wait(getpid());
			}
			

		}else{
			printf("ERROR: cmd = %s", cmd);
		}


	}
	return 0;
}
/* 
recursivePipe(char *, int) takes a command and index to knwo what command we are on
*/
int recursivePipe(char * cmd, int index){

	/* cmd = more */
	int pid, fd, copy;
	int ttyfd;
	char tty[64];
	

	if(index < 1){
		printf("index is -1... RETURN\n");
		return;
	}
	pipe(pd);

	// First fork a new child to stage the WRITING proc
	printf("forking to %s from %s\n", cmd, pipeCmds[index]);
	pid = fork();
	if(pid == 0){
		// Child
		close(pd[0]); // Close the child's read

		//if(index != (numCmds-2)){ // If we are not on the last command, make stdout be the pipe
			dup2(pd[1], 1);	// Place the WRITE end of the pipe into stdout so anything coming from the pipe
			close(pd[1]);
		//}
		copy = dup(1);
		close(1);
		gettty(tty);
		ttyfd = open(tty, O_WRONLY);
		if(ttyfd == 1){

		printf("exec(%s) from %s\n", cmd, pipeCmds[index]);
		close(1);
		dup(copy);
		close(copy);
	}

		/*copy = dup(0);
		close(0);
		gettty(tty);

		ttyfd = open(tty, O_RDONLY);
		if(ttyfd == 0){
		getc();
		close(0);
		dup(copy);
		close(copy);
		}*/

		exec(cmd);
	}else{
		// Parent
		printf("in parent: %s[%d]\n", pipeCmds[index], index);
		close(pd[1]); // Close the parent's write since more doesn't have to write

		//if(index != 0){ // If we are not on the first command, make stdin be the pipe
		
			dup2(pd[0], 0); // Place the READ end of pipe into stdin so that more's stdin comes from the pipe	
			close(pd[0]);
			
		//}

		printf("lets recurse!:  %s[%d]\n", pipeCmds[index], index);
	//	if(index > 0){
		recursivePipe(pipeCmds[index-2], index-1); // This will set up all other procs too with correct pipes
		//pid = wait(getpid());
		// }
		
		printf("done recurse!: %s[%d]\n", pipeCmds[index], index);

	}

}

int manageFileIO(char * cmd){
	char * pch = cmd;
	int type = 0;
	char *file;
	int fd;
	int pid;

	do{
		if(*pch == '>'){ // output to file
			printf("OUTPUT TO FILE\n");
			if(*(pch+1) == '>'){ // append to file
				printf("APPENDING\n");
				type = FILEAPPEND;
				file = pch + 3;
				
			
					

					
					fd = open(file, O_WRONLY | O_APPEND | O_CREAT); // Open file for write or create

					if(fd >= 0){
						*(pch-1) = '\0';
						printf("cmd = %s\n", cmd);
						printf("fd: %d, file = %s\n", fd, file);
						dup2(fd, 1);
						close(fd);
					}
		
				
				

			}else{ //just regular

				if(type == 0){
			type = FILEOUT;
			file = pch+2; // cmd="cat filename > newfile" then file = > + 2 which is 'newfile'
			
			fd = open(file, O_WRONLY | O_CREAT);
			
			if(fd >= 0){
			*(pch-1) = '\0';
			dup2(fd, 1);
			close(fd);

			}else{
				printf("Bad file descriptor in sh");
				getc();
			}
		}
		}

		}else if(*pch == '<'){ // Input from file
			type = FILEIN;
			file = pch + 2;
			printf("file = %s", file);

			fd = open(file, O_RDONLY);
			if(fd < 0){
				printf("bad fd in sh <");
				break;
			}
			*(pch-1) = '\0';
			dup2(fd, 0);

		}
		pch++;
	}while(*pch != '\0');

return type;


}
/*
 This function parses the cmd into different commands that use pipes to communicate
*/
int getPipeCmds(){
	char * pch;
	int count = 0;

	pch = strtok(cmd, ":"); // Right now I am using : instead of pipe character because it is broken on laptop

	// Parse string by : and count how many commands there are, then return that amount
	while(pch != 0){
		pipeCmds[count++] = pch;
		pch = strtok(0, ":");
	}
	pipeCmds[count] = 0;

	return count;
}
