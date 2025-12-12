/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * cp.c - /home/wujj/src/libs/src_ezutil
 *
 * Copyright (C) 2006 ezlibs.com, All Rights Reserved.
 *
 * $Id: cp.c, v 1.0.0.1 2006-03-11 13:23:35 wjj Exp $
 *
 * _detail_explain_.
 * 
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>


#ifndef lint
static char copyright[] =
    "@(#) Copyright (c) 2006\n\
    joy.woo.  All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char rcsid[] = "$Tools: cp.c,v 1.0 2006-3-11 13:22 joy.woo Exp $";
#endif /* not lint */

#define BSIZE   4*10*1024
/*10240*/

#define RAND_NUM_MIN 1
#define RAND_NUM_MAX BSIZE

/*
 * Copy from from to to.  Intended for use in system installation.
 */
int main(int argc, char *argv[])
{
	//extern int errno;
	register int from, to, record, rcc, wcc, bsize = BSIZE;
	char buf[BSIZE];
	
	int rand_num=0;
	
	srand((unsigned)time(NULL));  
	rand_num = (rand() % (RAND_NUM_MAX-RAND_NUM_MIN+1))+ RAND_NUM_MIN; 
	for (record=0; record<rand_num; record++)
	{}

	if (argc < 2)
	{
#ifndef lint
		printf("%s %s\n", copyright, rcsid);
#endif

		printf("%s <source> <dest>\n", argv[0]);

		return -1;
	}

	from = open(argv[1], O_RDONLY);
	to = open(argv[2], O_WRONLY|O_CREAT);

	for (record = 0;; ++record)
	{
		if (!(rcc = read(from, buf, bsize)))
		{
			//printf("Record finished=%d\n", rcc); // rcc==0
			
			break;
		}

		if (rcc < 0)
		{
			printf("Record %d: read error, errno=%d\n",
			       record, errno);
			break;
		}

		if (rcc != bsize)
		{
			if (record == 0)
			{
				bsize = rcc;
				printf("Block size set from input; %d bytes\n",
				       bsize);
			}
			else
			{
				printf("Record %d: read short; expected %d, got %d\n",
				       record, bsize, rcc);
			}
		}
		
		if ((wcc = write(to, buf, rcc)) < 0)
		{
			printf("Record %d: write error: errno=%d\n",
			       record, errno);
			break;
		}

		if (wcc < rcc)
		{
			printf("Record %d: write short; expected %d, got %d\n",
			       record, rcc, wcc);
			break;
		}
	}

	close(from);
	close(to);

	printf("copy completed: %d records copied\n", record);

        return 0;
}

