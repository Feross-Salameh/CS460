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

u16 getblk(u16 blk, char buf[ ])
{
	
	//u16 cylinder = 0, head = 0, sector = 0;
	//sector = (((blk % 18) % 9) * 2);
	//head = ((blk % 19) / 9);
	//cylinder = (blk / 18);
	readfd((blk / 18), ((blk % 18) / 9), (((blk % 18) % 9) * 2), buf);
}

INODE *getInode(int ino)
{
	int blk = 0;
	int offset;
	char buf[BLK];
	blk = (((ino - 1) / 8) + inodeStart);
	offset = ((ino - 1) % 8);
	getblk((u16)blk, buf);
	return (INODE *)buf + offset;
}

int getIno(INODE *tip, char buf[], char * target )
{
	int i = 0; 
	char *cp; 
	int tblk = 0;
	for ( i = 0; i < 12; i++)
	{
		tblk = tip->i_block[i];
		if(tblk <= 0)
			return -1;
		getblk((u16)tblk, buf);
		cp = buf;
		dp = (DIR *)buf;
		while(cp < buf + 1024)
		{
			
			if(!strncmp(dp->name, target, dp->name_len))
			{
				return dp->inode;
			}
			cp = cp + dp->rec_len;
			dp = (DIR *)cp;
			
		}

	}
	
	
}

main()
{ 
	int  i, iblk, ino = 0; u16 t = 0;
	char buf1[BLK], buf2[BLK];
	char input[64];
	/* read blk#2 to get group descriptor 0 */
	getblk((u16)2, buf1);
	gp = (GD *)buf1;
	inodeStart = (u16)gp->bg_inode_table; // typecast u32 to u16


	getblk((u16)inodeStart, buf1);
	ip = (INODE *)buf1 + 1; 
	ino = getIno(ip, buf2, "boot");
	if(ino <= 0)
		return;
	ip = getInode(ino);
	// asking for what is file.
	prints("Boot: ");
	gets(input);
	ino = getIno(ip, buf2, input);
	//ino = getIno(ip, buf2, "mtx");
	if(ino <= 0) // error in getting correct ino
		return;
	
	ip = getInode(ino);
	
	getblk((u16)ip->i_block[12], buf2);
	ptr = (int *)buf2;
	setes(0x1000);
	
	for(i = 0; i < 12; i++)
	{
		getblk((u16)ip->i_block[i], 0);
		inces();
	}
	
	while(*ptr)
	{
		getblk((u16)*ptr, 0);
		inces();
		ptr++;
	}
	
	
}  
