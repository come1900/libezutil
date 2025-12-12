/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * des.c - /home/wujj/src/libs/tutorial_ezutil
 *
 * Copyright (C) 2008 ezlibs.com, All Rights Reserved.
 *
 * $Id: des.c 5884 2008-7-16 11:00:42Z wu_junjie $
 *
 *  Explain:
 *     des user
 *
 *  Update:
 *     2008-7-16 11:00:24 WuJunjie 10221 Create
 *
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ezutil/str_opr.h>
#include <ezutil/des.h>

int main (int argc, char **argv)
{
	int iret;
	const  char * const pOrig = "hellostring";
	// const  char * const pOrig = "marryhadalittlesheep";
	const  char * const pPwd = "12345678";
	char buf[64];
	char buf2[64];

	//--------------------------
	memset( buf, 0, sizeof(buf));
	printf("orig:%s, passwd:%s\n", pOrig, pPwd);
	iret = ez_des_ecb_encrypt((unsigned char *)buf, (unsigned char *)pOrig, strlen(pOrig), pPwd, strlen(pPwd));
	printf("des_ecb_encrypt:%d\n", iret);
	// hellostring 12345678 -> 824370D0D38C9E94180069638F26CB54
	dumpBuffer(stdout,
	           (unsigned char*)buf,
	           sizeof(buf),
	           SHOW_ASCII | SHOW_BINAR | SHOW_LINER);

	//--------------------------
	iret = ez_des_ecb_decrypt((unsigned char *)buf, (unsigned char *)buf, iret, pPwd,strlen(pPwd));
	printf("des_ecb_decrypt:%d\n", iret);
	dumpBuffer(stdout,
	           (unsigned char*)buf,
	           sizeof(buf),
	           SHOW_ASCII | SHOW_BINAR | SHOW_LINER);

	//--------------------------
	memset( buf, 0, sizeof(buf));
	memset( buf2, 0, sizeof(buf2));
	memcpy(buf2, pOrig, strlen(pOrig));
	printf("orig:%s, passwd:%s\n", pOrig, pPwd);
	iret = DesEncrypt(( char *)buf, ( char *)buf2, strlen(pOrig), pPwd, strlen(pPwd));
	printf("DesEncrypt:%d\n", iret);
	// hellostring 12345678 -> 1b456173f947f0 04 69 eb 9c cb be 32 98 ab
	dumpBuffer(stdout,
	           (unsigned char*)buf,
	           sizeof(buf),
	           SHOW_ASCII | SHOW_BINAR | SHOW_LINER);

	//--------------------------
	iret = DesDecrypt(( char *)buf, ( char *)buf, iret, pPwd, strlen(pPwd));
	printf("DesDecrypt:%d\n", iret);
	dumpBuffer(stdout,
	           (unsigned char*)buf,
	           sizeof(buf),
	           SHOW_ASCII | SHOW_BINAR | SHOW_LINER);

	return 0;
}
