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
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <ezutil/sha256.h>
#include <ezutil/str_opr.h>

#include <ezutil/base58.h>
#include <ezutil/libbase58.h>

#define _DEBUG_THIS
#include <ezutil/ez_def_devel_debug.h>

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

////
	size_t bel = base58_encode_length(SHA256_BLOCK_SIZE);
	ARG_USED(bel);

	size_t encode_out_size = bel+1;
	char *encode_out = (char *) malloc(encode_out_size);
    memset(encode_out, 0, encode_out_size);
    bool base58_encode_iret = b58enc(encode_out, &encode_out_size, (char *)buf_sha256, SHA256_BLOCK_SIZE);
	ARG_USED(base58_encode_iret);
    printf("encode_out:%s\n", encode_out);
    printf("bel:%ld, encode_out_size:%ld\n", bel, encode_out_size);

    size_t bdl = base58_decode_length(encode_out_size/*含最后的0*/-1);
	ARG_USED(bdl);
    size_t decode_out_size = bdl;
	char *decode_out = (char *) malloc(decode_out_size);
    bool base58_decode_iret = b58tobin(decode_out, &decode_out_size, encode_out, encode_out_size-1);
    printf("bdl:%ld, decode_out_size:%ld\n", bdl, decode_out_size);
    (void)&base58_decode_iret;

	dumpBuffer(stdout,
			   (unsigned char *)decode_out,
			   decode_out_size,
			   SHOW_ASCII | SHOW_BINAR | SHOW_LINER);

    free(encode_out);
    free(decode_out);

////


	return (0);
}
