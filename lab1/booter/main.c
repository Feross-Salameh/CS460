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
	
	u16 cylinder = 0, head = 0, sector = 0;
	sector = ((blk % 18) % 9) * 2;
	head = (blk % 19) / 9;
	cylinder = (blk / 18);
	readfd(cylinder, head, sector, buf);
}

INODE *getInode(int ino)
{
	int blk = 0;
	int offset;
	char buf[BLK];
	blk = ((ino - 1) / 8) + gp->bg_inode_table;
	offset = (ino - 1) % 8;
	getblk((u16)blk, buf);
	putc('3');
	return (INODE *)buf + offset;
}

int getIno(INODE *tip, char buf[], char * target)
{
	int i = 0; 
	char *cp; 
	int tblk = 0;
	for ( i = 0; i < 12; i++)
	{
		tblk = tip->i_block[i];
		if(tblk <= 0)
		{
			prints("did not found target: "); prints(target); prints(", returning\n");
			return -1;
		}
		getblk((u16)tblk, buf);
		cp = buf;
		dp = (DIR *)buf;
		while(cp < buf + 1024)
		{
			if(!strncmp(dp->name, target, dp->name_len))
			{
				
				prints("Found: "); prints(target); prints(" folder\n");
				return dp->inode;
			}
			cp = cp + dp->rec_len;
			dp = (DIR *)cp;
			
		}

	}
	
	
}

main()
{ 
	u16  i, iblk; int ino = 0; INODE *tip;
	char buf1[BLK], buf2[BLK];
	
	prints("booter start\n");  
	/* read blk#2 to get group descriptor 0 */
	getblk((u16)2, buf1);
	gp = (GD *)buf1;
	iblk = (u16)gp->bg_inode_table; // typecast u32 to u16


	getblk((u16)iblk, buf1);
	ip = (INODE *)buf1 + 1; 
	ino = getIno(ip, buf2, "boot");
	tip = getInode(ino);
  
  
  
  
}  
