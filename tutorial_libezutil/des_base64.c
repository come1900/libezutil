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
#include <ezutil/base64.h>


int main (int argc, char **argv)
{
	int iret;
	const  char * const pOrig = "w886801w";
	const  char * const pPwd = "DESede";
	
	char buf[1024] = {'\0'};
	char buf2[1024] = {'\0'};

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

	iret = ez_base64encode(buf2, buf, iret);
	printf("buf2:%s\n", buf2);
	//--------------------------


	return 0;
}
