/* 
* Descent 3 
* Copyright (C) 2024 Parallax Software
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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
	struct timeval tv;
	tv.tv_sec = millis / 1000;
	tv.tv_usec = (millis % 1000) * 1000;
	select(0,NULL,NULL,NULL,&tv);
}

char *strupr(char *string)
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
}
