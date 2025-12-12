/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * md5er.c - /home/wujj/src/libs/tutorial_ezutil
 *
 * Copyright (C) 2008 ezlibs.com, All Rights Reserved.
 *
 * $Id: md5er.c 5884 2008-7-14 11:00:42Z wu_junjie $
 *
 *  Explain:
 *     md5 user
 *
 *  Update:
 *     2008-7-14 11:00:24 WuJunjie 10221 Create
 *
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <stdio.h>
#include <stdlib.h>

#include <ezutil/str_opr.h>
#include <ezutil/md5.h>


int main (int argc, char **argv)
{

	char bufOut[64];
	const char * const pOrig = "hellostr";

	//--------------------------
	memset(bufOut, 0, sizeof(bufOut));
	ez_md5_calc((unsigned char *)bufOut, (unsigned char *)pOrig, strlen(pOrig));
	printf("\nOrig str:%s\n", pOrig);
	printf("ez_md5_calc\n");
	dumpBuffer(stdout,
	           (unsigned char*)bufOut,
	           sizeof(bufOut),
	           SHOW_ASCII | SHOW_BINAR | SHOW_LINER);

	//--------------------------
	memset(bufOut, 0, sizeof(bufOut));
	ez_md5_calc_hexstr((unsigned char *)bufOut, (unsigned char *)pOrig, strlen(pOrig));
	printf("\nOrig str:%s\n", pOrig);
	printf("ez_md5_calc_hexstr\n%s\n", bufOut);
	printf("\tThis md5 alo is:%s\n", strcmp("F02FEF205403990AA684BA3999C83475", bufOut)==0?"OK":"NOK");

	/*
		hellostr
		F02FEF205403990AA684BA3999C83475
	*/
	//--------------------------
	memset(bufOut, 0, sizeof(bufOut));
	ez_md5_calc_8ascii((unsigned char *)bufOut, (unsigned char *)pOrig, strlen(pOrig));
	printf("\nOrig str:%s\n", pOrig);
	printf("ez_md5_calc_8ascii\n%s\n", bufOut);

	//--------------------------
	memset(bufOut, 0, sizeof(bufOut));
	ez_md5_calc_8ascii_digit((unsigned char *)bufOut, 7, (unsigned char *)pOrig, strlen(pOrig));
	printf("\nOrig str:%s\n", pOrig);
	printf("ez_md5_calc_8ascii_digit\n%s\n", bufOut);
	//--------------------------
        memset(bufOut, 0, sizeof(bufOut));
	ez_super_passwd_device((signed char *)bufOut, 7);
	printf("ez_super_passwd_device\n%s\n", bufOut);

	return 0;
}
