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
	u16 block = blk;
	u16 cylinder = 0, head = 0, sector = 0;
	//cylinder = block % 5;
	//uh = (b % 36) / 2;
	//c = (blk / 36);
}




GD    *gp;
INODE *ip;
DIR   *dp;

main()
{ 
  u16  i, iblk;
  char buf1[BLK], buf2[BLK];

  prints("booter start\n\r");  

  /* read blk#2 to get group descriptor 0 */
  getblk((u16)2, buf1);
  gp = (GD *)buf1;
  iblk = (u16)gp->bg_inode_table; // typecast u32 to u16

  prints("inode_block="); putc(iblk+'0'); getc();
  /******** write C code to do these: ********************
  (1).read first inode block into buf1[ ]

  (2).let ip point to root INODE (inode #2)
   
  (3).For each DIRECT block of / do:
          read data block into buf2[ ];
          step through the data block to print the names of the dir entries 
  
  (4).prints("\n\rAll done\n\r");
  ******************************************************/
  
}  
