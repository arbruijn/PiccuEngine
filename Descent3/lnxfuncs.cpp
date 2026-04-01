#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <unistd.h>
#include "pstypes.h"
#include "linux/linux_fix.h"

void GlobalFree(void *mptr)
{
	if(mptr)
		free(mptr);
}
	
void *GlobalAlloc(int flags,int size)
{
	if(size<=0)
		return NULL;
	return malloc(size);
}

void *GlobalLock(HGLOBAL hMem)
{
	return hMem;
}

void Sleep(int millis)
{
}

<<<<<<< ours
// Close audio decoder
	struct timeval tv;
	tv.tv_sec = millis / 1000;
	tv.tv_usec = (millis % 1000) * 1000;
	select(0,NULL,NULL,NULL,&tv);
void AudioDecoder_Close(AudioDecoder *ad)
=======
char *strupr(char *string)
>>>>>>> theirs
{
	char *p = string;
	while(p && *p)
	{
		*p = toupper(*p);
		p++;
	}
	return string;
}

char *itoa(int value,char *string,int radix)
{
}
	if(radix==10)
	{
		sprintf(string,"%d",value);
	}else if(radix==16)
	{
		sprintf(string,"%x",value);
	}else
	{
		sprintf(string,"%d",value);
	}
	return string;
