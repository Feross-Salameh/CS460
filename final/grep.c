/* grep.c */

// It works ok for now

#include "ucode.c"

int main(int argc, char * argv[]){
	// argv[1] = pattern argv[2] = file
	int fd, nread;;
	char * pattern;
	char buf[1024];
	char * pch;
	int i = 0;
	int numLines;
	char ch;

	if(argc > 1){
		// Open up the file
		//printf("grp argc = %d\n", argc);
		if(argc == 2){
			fd = 0;
		}else{
			fd = open(argv[2], O_RDONLY);// First parse by new line character
		}

		if(fd >= 0){

		pattern = argv[1];

		//readLine(fd, buf);
		//printf("about to read from %d\n", fd);
		i = 0;
		while(nread = read(fd, &ch, 1)){
			
			// First parse by new line character
			/*
			pch = strtok(buf, "\n");
			while(pch != 0){
				lines[i++] = pch;
				pch = strtok(0, "\n");
			}
			lines[i] = 0;
			numLines = i;

			i=0;
			while(i< numLines){
			printf("lines[%d] = %s\n", i, lines[i]);
			//checkLine(lines[i], pattern);
			i++;
			}*/
			buf[i++] = ch;
			if(ch == '\n'){
				buf[i] = '\r';
				buf[i+1] = 0;
				checkLine(buf, pattern);
				i = 0;
			}

			
		}
		}else{
			printf("bad fd in grep\n");

		}
		


	}else{
		printf("usage: grep pattern filename\n");
	}
	return 0;
}

int checkLine(char * line, char * pattern){
	int i = 0, j = 0;
	int start = 0;
	int len;
	int patlen = strlen(pattern);
	len = strlen(line);

	while(i < len){

		if(line[i] != pattern[j]){ // Look for a match of first char

			i++;
			continue;
		}else{
			// we found a match
			//printf("found match.. %c[%d] == %c[%d]\n", line[i], i, pattern[j], j);
			start = i;
			i++;
			j++;

			//printf("checking rest...\n");
			while(j < patlen){
				//printf("is %c == %c?\n", line[i], pattern[j]);
				if(line[i++] != pattern[j++]){
					//printf("ohh poooo its not a match");
					i = start+1;
					j = 0;
					break;
				}
			}

			if(j == patlen){
				printf("%s", line);
				break;
			}


		}


	}
}