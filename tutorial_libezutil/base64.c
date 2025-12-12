/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * base64.c - /home/wujj/src/libs/tutorial_ezutil
 *
 * Copyright (C) 2008 ezlibs.com, All Rights Reserved.
 *
 * $Id: base64.c 5884 2008-7-15 16:17:42Z wu_junjie $
 *
 *  Explain:
 *     base64 user
 *
 *  Update:
 *     2008-7-15 16:17:24 WuJunjie 10221 Create
 *
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <stdio.h>
#include <string.h>

#include <ezutil/base64.h>


int main (int argc, char **argv)
{

	int iret;
	char bufOut[64];
	char buftmp[64];
	const char * const pOrig = "admin:admin";

	//--------------------------
	memset(bufOut, 0, sizeof(bufOut));
	iret = ez_base64encode_len(strlen(pOrig));
	printf("\nOrig str:%s\n", pOrig);
	printf("ez_base64encode_len:%d\n", iret);

	//--------------------------
	memset(bufOut, 0, sizeof(bufOut));
	iret = ez_base64encode(bufOut, pOrig, strlen(pOrig));
	printf("\nOrig str:%s\n", pOrig);
	printf("ez_base64encode:%d:[%s]\n", iret, bufOut);
	printf("\tThis base64 alo is:%s\n", strcmp("aGVsbG9zdHI=", bufOut)==0?"OK":"NOK");

	//--------------------------
	//memset(bufOut, 0, sizeof(bufOut));
	iret = ez_base64decode_len(bufOut);
	printf("\nOrig str:%s:%ld\n", pOrig, strlen(pOrig));
	printf("ez_base64decode_len:%d\n", iret);

	//--------------------------
	memset(buftmp, 0, sizeof(buftmp));
	iret = ez_base64decode(buftmp, bufOut);
	printf("\nbase64 str:%s\n", bufOut);
	printf("ez_base64decode:%d:[%s]\n", iret, buftmp);

	return 0;
}

