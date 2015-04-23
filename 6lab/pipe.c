show_pipe(PIPE *p)
{
   int i, j;
   if(!p)
   {
	   printf("Show_pipe: pipe is invalid.\n");
	   return;
   }
   printf("------------ PIPE CONTENETS ------------\n");     
   printf("	buf:%s", p->buf);
   printf("	head: %d, tail: %d, data: %d, room: %d", p->head, p->tail, p->data, p->room);
   printf(" nreader: %d, nwriter: %d\n", p->nreader, p->nwriter);
   printf("\n----------------------------------------\n");
}

char *MODE[ ]={"READ_PIPE ","WRITE_PIPE"};

int pfd()
{
	int i;
	printf(" fd \t type \t mode  \n");
	printf("----\t------\t-------\n");
	for (i = 0; i < NFD; i++)
	{
		if (running->fd[i])
		{
			printf(" %d \t PIPE \t %s\n", i, MODE[running->fd[i]->mode]);
		}
	}
}
//============================================================


int read_pipe(int fd, char *buf, int n)
{
	int i = 0;

	PIPE* pp = running->fd[fd]->pipe_ptr;
	while (1)
		{
			while (pp->data > 0 && i < n)
			{
				if (pp->tail == PSIZE) { pp->tail = 0; }
				put_byte(pp->buf[pp->tail++], running->uss, buf+(i++));
				pp->data--;
				pp->room++;
			}
			kwakeup(&(pp->room));
			show_pipe(pp);
			if (i == n)
			{
				return n;
			}
			else
			{
				ksleep(&(pp->data));
				return;
			}
		}
}

int write_pipe(int fd, char *buf, int n)
{
	// your code for write_pipe()
	int i = 0;
	PIPE* pp = running->fd[fd]->pipe_ptr;
	
	while(1)
	{
		while(pp->room >= 1 && i < n)
		{
			if(pp->head == PSIZE)
				pp->head = 0;
			pp->buf[pp->head++] = get_byte(running->uss, buf+(i++));
			pp->room--;
			pp->data++;
		}
		kwakeup(&(pp->data));
		show_pipe(pp);
		if(i == n)
			return n;
		else
			ksleep(&(pp->room));
	}
}

int kpipe(int pd[2])
{
  /* create a pipe 
	 fill pd[0] pd[1] (in USER mode!!!) with descriptors */
	
	int i, o, p;
	pd[0] = 0;
	pd[1] = 0;
	i = 0;
	o = 0;

	// Find two free OFTs
	while (o != 2)
	{
		if (i == NFD)
		{ 
			pd[0] = 0;
			pd[1] = 0;
			printf("No available file descriptors\n"); return -1; 
		}

		if (oft[i].refCount == 0)
		{
			pd[o++] = i;
		}

		i++;
	}

	// Find free pipe
	p = 0;
	while (p <= NPIPE)
	{
		if (p == NPIPE)
		{
			pd[0] = 0;
			pd[1] = 0;
			printf("No available pipe\n"); return -1;
		}

		if (pipe[p].busy == 0)
		{
			pipe[p].head = 0; 
			pipe[p].tail = 0;
			pipe[p].data = 0; 
			pipe[p].room = PSIZE;
			pipe[p].nreader = 1;
			pipe[p].nwriter = 1;
			pipe[p].busy = 1;
			
			// reader
			oft[pd[0]].refCount = 1;
			oft[pd[0]].mode = 0;
			running->fd[pd[0]] = &oft[pd[0]];
			running->fd[pd[0]]->pipe_ptr = &pipe[p];

			// writer
			oft[pd[1]].refCount = 1;
			oft[pd[1]].mode = 1; 
			running->fd[pd[1]] = &oft[pd[1]];
			running->fd[pd[1]]->pipe_ptr = &pipe[p];

			put_byte(pd[0], running->uss, pd);
			put_byte(pd[1], running->uss, pd+1);
			return 0;
		}

		p++;
	}

	return -1;
}

int close_pipe(int fd)
{
  OFT *op; PIPE *pp;

  printf("proc %d close_pipe: fd=%d\n", running->pid, fd);

  op = running->fd[fd];
  running->fd[fd] = 0;					// clear fd[fd] entry 

  if (op->mode == READ_PIPE){
	  pp = op->pipe_ptr;
	  pp->nreader--;					// dec n reader by 1

	  if (--op->refCount == 0){			// last reader
		  if (pp->nwriter <= 0){		// no more writers
			  pp->busy = 0;				// free the pipe   
			  return;
		  }
	  }
	  kwakeup(&pp->room);				// wakeup any WRITER on pipe 
	  return;
  }
  
  if (op->mode == WRITE_PIPE){
	  pp = op->pipe_ptr;
	  pp->nwriter--;					// dec n writer by 1

	  if (--op->refCount == 0){			// last reader
		  if (pp->nreader <= 0){		// no more writers
			  pp->busy = 0;				// free the pipe   
			  return;
		  }
	  }
	  kwakeup(&pp->data);				// wakeup any READER on pipe 
	  return;
  }

}
