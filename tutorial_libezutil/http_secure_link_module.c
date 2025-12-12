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
#include "ezutil/std_md5.h"
#include <ezutil/base64.h>

int main (int argc, char **argv)
{
    int iret = 0;
    char bufOut[64];
    const char * const pAccessUrl = "/pssix.html";
    const char * const pAccessSec = " jsecret";
    const char * const pRemote_addr = "192.168.9.148";

    time_t curTime = time(NULL);
    time_t expTime = curTime+300;

    char strOrig[128] = {0};

    snprintf(strOrig, sizeof(strOrig)-1, "%ld%s%s%s", expTime, pAccessUrl, pRemote_addr, pAccessSec);
    //strcpy(strOrig, "1624702964/pssix.html192.168.9.148 jsecret");

    //--------------------------
    memset(bufOut, 0, sizeof(bufOut));
    std_MD5_CTX ctx;
    unsigned char outputtemp[32]={0};
    //int iPrint;

    std_MD5_Init(&ctx);

    std_MD5_Update(&ctx, strOrig, strlen(strOrig));

    std_MD5_Final(outputtemp, &ctx);

    //        	printf("\nOrig str:%s\n", strOrig);
    //	printf("ez_md5_calc\n");
    //	dumpBuffer(stdout,
    //	           (unsigned char*)outputtemp,
    //	           sizeof(outputtemp),
    //	           SHOW_ASCII | SHOW_BINAR | SHOW_LINER);

    //--------------------------

    iret = ez_base64encode_len(16);
    char __Md5Out[16];
    memcpy(__Md5Out, outputtemp, 16);
    printf("iret:%d\n", iret);
    memset(bufOut, 0, sizeof(bufOut));
    iret = ez_base64encode(bufOut, __Md5Out, 16);
    printf("ez_base64encode:%d:[%s]\n", iret, bufOut);

    printf("http://192.168.9.238:18080/pssix.html?md5=%s&expires=%ld\n", bufOut , expTime);

    //--------------------------

    return 0;
}

/*
echo -n '1624702964/pssix.html192.168.9.148 jsecret' | \
    openssl md5 -binary | openssl base64 | tr +/ -_ | tr -d =
 
http://192.168.9.238:18080/pssix.html?md5=4YAk6XwsoRURQ4fLpODLjQ&expires=1624702964
 
*/
