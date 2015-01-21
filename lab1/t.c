/************** t.c file **************************/
int prints(char *s)
{
  return 1;
}

int gets(char s[ ])
{
  return 1;
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
   prints("return to assembly and hang\n\r");
}
