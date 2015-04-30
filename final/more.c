/* more.c */

#include "ucode.c"

int main(int argc, char * argv[]){
	
	int fd, nread;
	char buf[128];
	int enterKey = 0;
	int i = 0;
	int totalcount = 0;
	char ch;
	int copy;
	char tty[256];
	int ttyfd;
	printf("Entered more\n");

	if(argc == 1){
		
		fd = 0;
	}else if(argc > 1){

		fd = open(argv[1], O_RDONLY);
	}

		if(fd >= 0){
			
			while((nread = read(fd, buf, 128)) > 0){
				i = 0;
				while(i < nread){
					putc(buf[i]);
					if(buf[i] == '\n'){

						if(enterKey == 1){
							enterKey = 0;
							copy = dup(0);
							close(0);
							gettty(tty);

							ttyfd = open(tty, O_RDONLY);
							if(ttyfd == 0){
							ch = getc();
							close(0);
							dup(copy);
							close(copy);
							}

							if(ch == 13){
								// Enter key was hit
								enterKey = 1;
							}
						}
							if(++totalcount % 24 == 0){
								copy = dup(0);
								close(0);
								gettty(tty);

								ttyfd = open(tty, O_RDONLY);
								if(ttyfd == 0){
								ch = getc();
								close(0);
								dup(copy);
								close(copy);
								}
								
								if(ch == 13){
									// Enter key was hit
									enterKey = 1;
								}
							}

						
					}
					i++;
				}
				
			}

		}else{
			printf("bad file");
		}

	return 0;
}
