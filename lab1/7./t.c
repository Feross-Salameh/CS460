#include "ext2.h"


int prints(char *s)
{
   	char *t = s;
	while(*t != '\0')
	{
		putc(*t);
		if(*t == '\n')
			putc('\r');
		t++;
	}
}

u16 getblk(u16 blk, char buf[ ])
{
	
	//u16 cylinder = 0, head = 0, sector = 0;
	//sector = ((block % 18) % 9) * 2;
	//head = (block % 19) / 9;
	//cylinder = (block / 18);
	//readfd(cylinder, head, sector, buf);
	readfd(blk/18, ((blk)%18)/9, (((blk)%18)%9)*2, buf);
}

u16 printDIR(char buf[])
{
	int i = 0; 
	char *cp; 
	int tblk = 0;
	prints("In printDIR\n");
	//for ( i = 0; i < 12; i++)
	//{
		prints("in loop\n");
		tblk = ip->i_block[0];

		prints("getting new block\n");
		getblk((u16)tblk, buf);
		cp = buf;
		dp = (DIR *)buf;
		while(cp < buf + 1024)
		{
			
			prints(dp->name);
			prints(" ");
			cp = dp + dp->rec_len;
			dp = cp;
			
		}
		
	//}
	
	
}


main()
{ 
  u16  i, iblk;
  char buf1[BLK], buf2[BLK];

  prints("booter start\n");  

  /* read blk#2 to get group descriptor 0 */
  getblk((u16)2, buf1);
  gp = (GD *)buf1;
  iblk = (u16)gp->bg_inode_table; // typecast u32 to u16

  prints("inode_block= "); putc(iblk+'0');// getc();
  prints("\n");
  /******** write C code to do these: ********************
  (1).read first inode block into buf1[ ]

  (2).let ip point to root INODE (inode #2)
   
  (3).For each DIRECT block of / do:
          read data block into buf2[ ];
          step through the data block to print the names of the dir entries 
  
  (4).prints("\n\rAll done\n\r");
  ******************************************************/
  prints("getting root inode.\n");
  getblk((u16)iblk, buf1);
  ip = (INODE *)buf1 + 1; 
  //printDIR(buf2);
   getblk((u16)ip->i_block[0], buf2);
  prints("past getblk\n");
  
  
}  
