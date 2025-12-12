/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * file_sha256.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: file_sha256.c 5884 2023-04-30 04:33:45Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Chk:
 *     [wujj@CoolPc tutorial_libezutil]$ ./file_sha256.o file_sha256.o
 *     ./file_sha256.o file_sha256.o
 *     0001: 4a 7e 1a fc b7 9b 8a c9 d1 35 6a 43 57 db 32 37 ; J~.......5jCW.27
 *     0002: fd 15 8a ca df 60 78 eb 6f 80 58 fa ba c2 c3 da ; .....`x.o.X.....
 *     [wujj@CoolPc tutorial_libezutil]$ sha256sum file_sha256.o
 *     4a7e1afcb79b8ac9d1356a4357db3237fd158acadf6078eb6f8058fabac2c3da  file_sha256.o
 *
 *  Update:
 *     2023-04-30 04:33:45  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include <ezutil/sha256.h>
#include <ezutil/str_opr.h>

#define _defDefaultBuffSize (4 * 10 * 1024)

int main(int argc, char *argv[])
{
	register int from = -1;
	register int record = 0;
	register int rcc = 0;
	register int bsize = _defDefaultBuffSize;
	char buf[_defDefaultBuffSize] = {0};

	if (argc < 2)
	{
		printf("%s <file>\n", argv[0]);

		return -1;
	}
	printf("%s %s\n", argv[0], argv[1]);
	from = open(argv[1], O_RDONLY);

	if (from < 0)
	{
		printf("Open:%s failed(%d):%s\n", argv[1], errno, strerror(errno));
		return -2;
	}

	SHA256_CTX ctx;
	sha256_init(&ctx);
	unsigned char buf_sha256[SHA256_BLOCK_SIZE] = {0};

	for (record = 0;; ++record)
	{
		if (!(rcc = read(from, buf, bsize)))
		{
			// printf("Record finished=%d\n", rcc); // rcc==0

			break;
		}

		if (rcc < 0)
		{
			printf("Record %d: read error, errno=%d\n",
				   record, errno);
			break;
		}

		sha256_update(&ctx, (unsigned char *)buf, rcc);
	}

	sha256_final(&ctx, buf_sha256);

	dumpBuffer(stdout,
			   (unsigned char *)buf_sha256,
			   sizeof(buf_sha256),
			   SHOW_ASCII | SHOW_BINAR | SHOW_LINER);

	if (from >= 0)
		close(from);

	return (0);
}
