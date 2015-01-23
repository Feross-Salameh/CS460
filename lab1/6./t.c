/************** t.c file **************************/
int prints(char *s)
{	
	char *t = s;
	while(*t != '\0')
	{
		putc(*t);
		t++;
	}
	putc('\n');
	putc('\r');
}

int gets(char s[ ])
{
	*s = getc();
	putc(*s);
	while(*s != '\r')
	{
		s++;
		*s = getc();
		putc(*s);
		
	}
	
}

main()
{
   char name[64];
   
   while(1){
     prints("What's your name? ");
     gets(name);
     if (name[0]==0)
        break;
     prints("Welcome "); prints(name); prints("\n\r");
   }
   //prints("return to assembly and hang\n\r");
   
   
}
