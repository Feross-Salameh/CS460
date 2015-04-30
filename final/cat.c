
#include "ucode.c"



int main(int argc, char * argv[]) {

	int fd, nread;
	char buf;
	int i = 0;
	char test;
	int copy, ttyfd;
	char tty[128];
	printf("Entered cp\n");
		
	if(argc > 1)
	{

		fd = open(argv[1], O_RDONLY);

	}
	else
	{
		fd = 0;
	}

		if(fd >= 0)
		{
			
			while(nread = read(fd, &buf,1))
			{
				if(buf != '\r')
				{
					putc(buf);
				}

				/*if(fd == 0){
				if(buf == '\r')
					putc('\n');
				}*/
			}

		}
		else
		{
			printf("bad file\n");
		}

	return 0;
}
