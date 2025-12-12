/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * test_ez_md5_calc_ascii_digit.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: test_ez_md5_calc_ascii_digit.c 5884 2016-08-08 08:47:28Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2016-08-08 08:47:28  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <../src/md5.h>

#define def_Number_Out 9

int main(int argc,char * argv[])
{
    unsigned char * pret;

    unsigned char strOutput[def_Number_Out+1] = {0};
    unsigned int iOutLen=def_Number_Out;
    unsigned char *strInput = (unsigned char *)"9ee35c852630d0a223cfb42451180b97fc67bbab";
    unsigned int inlen = (unsigned int )strlen(strInput);

    printf("strInput(%d):%s\n", inlen, strInput);

    pret = ez_md5_calc_ascii_digit(strOutput,
                                   iOutLen,
                                   strInput,
                                   inlen);
    if (pret!=NULL)
    {
        printf("strOutput(%d):%s\n", strlen(strOutput), strOutput);
    }
    else
    {
        printf("ez_md5_calc_ascii_digit failed\n");
    }

    return 0;
}

