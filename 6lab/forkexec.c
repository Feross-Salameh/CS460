int copyImage(u16 child_segment)
{
  // your copyimage function
}

int fork()
{
  int i, pid;  u16 segment;

  PROC *p = kfork(0);   // kfork() but do NOT load any Umode image for child 
  if (p == 0)           // kfork failed 
    return -1;

  segment = (p->pid+1)*0x1000;
  copyImage(segment);

  	for (i=1; i<=12; i++){         // write 0's to ALL of them
		 put_word(0, segment, -2*i);
	 }
	 
	 put_word(0x0200,   segment, 0x10000-2*1);   /* flag */  
	 put_word(segment,  segment, 0x10000-2*2);   /* uCS */  
	 put_word(segment,  segment, 0x10000-2*11);  /* uES */  
	 put_word(segment,  segment, 0x10000-2*12);  /* uDS */   

	/* initial USP relative to USS */
	 p->usp = -2*12; 
	 p->uss = segment;

   /**** Copy file descriptors ****/
   for (i=0; i<NFD; i++){
      p->fd[i] = running->fd[i];
      if (p->fd[i] != 0){
          p->fd[i]->refCount++;
          if (p->fd[i]->mode == READ_PIPE)
              p->fd[i]->pipe_ptr->nreader++;
          if (p->fd[i]->mode == WRITE_PIPE)
	      p->fd[i]->pipe_ptr->nwriter++;
      }
   }

   return(p->pid);
}

int exec(char *filename)
{
  // your exec function
}

