/********** t.c file ******************/

#include "ext2.h"  /* download this from LAB1 */
typedef struct ext2_group_desc  GD;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;

#define BLK 1024

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

int blk2cyl(u16 blk)
{
	return  blk / 36;
}

int blk2hd(u16 blk)
{
	return 
}

u16 getblk(u16 blk, char buf[ ])
{
   // convert blk to CHS; then call readfd(c,h,s, buf);
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
  prints("Starting while");
  while(1)
  {
	  
  }
  
}  
