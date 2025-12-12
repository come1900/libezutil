/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ez_md5.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ez_md5.c 5884 2017-05-18 05:15:05Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2017-05-18 05:15:05  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <string.h>
#include <stdio.h>

#include "std_md5.h"

#include "ez_md5.h"

//#define EZ_STD_MD5_RESULT_LEN 16 // Byte
/*inline */int ez_cal_std_md5(char *dst, size_t dst_len/*16*/, const char *src, size_t src_len)
{
    if (dst==NULL
        || dst_len<EZ_STD_MD5_RESULT_LEN
        || src==NULL
        || src_len<1)
    {
        return -1;
    }

    std_MD5_CTX ctx;

    std_MD5_Init(&ctx);

    std_MD5_Update(&ctx, src, src_len);

    std_MD5_Final((unsigned char *)dst, &ctx);

    return 0;
}

//转换为大写字符串的字符长度， 缓存长度注意+1
//#define EZ_STD_MD5_STRING_LEN_UP_32B 32 // Byte
int ez_cal_std_md5_32b_up_string(char *dst, size_t dst_len/*32*/, const char *src, size_t src_len)
{
    if (dst==NULL
        || dst_len<=EZ_STD_MD5_STRING_LEN_UP_32B
        || src==NULL
        || src_len<1)
    {
        return -1;
    }
    int iret;

    unsigned char out[EZ_STD_MD5_RESULT_LEN] = {0};
    iret = ez_cal_std_md5((char *)out, EZ_STD_MD5_RESULT_LEN, src, src_len);

    if (iret < 0)
    {
        return -2;
    }

    // display result
    int ii;
    dst[0] = '\0';
    char buf_bytes[4] = {'\0'};
    for (ii = 0; ii<EZ_STD_MD5_RESULT_LEN; ii++)
    {
        buf_bytes[0] = '\0';

        sprintf(buf_bytes, "%02X",out[ii]);
        strcat(dst, buf_bytes);
    }

    return 0;
}

//转换为大写字符串的字符长度， 缓存长度注意+1
//#define EZ_STD_MD5_STRING_LEN_UP_32B 32 // Byte
int ez_cal_std_md5_32b_low_string(char *dst, size_t dst_len/*32*/, const char *src, size_t src_len)
{
    if (dst==NULL
        || dst_len<=EZ_STD_MD5_STRING_LEN_UP_32B
        || src==NULL
        || src_len<1)
    {
        return -1;
    }
    int iret;

    unsigned char out[EZ_STD_MD5_RESULT_LEN] = {0};
    iret = ez_cal_std_md5((char *)out, EZ_STD_MD5_RESULT_LEN, src, src_len);

    if (iret < 0)
    {
        return -2;
    }

    // display result
    int ii;
    dst[0] = '\0';
    char buf_bytes[4] = {'\0'};
    for (ii = 0; ii<EZ_STD_MD5_RESULT_LEN; ii++)
    {
        buf_bytes[0] = '\0';

        sprintf(buf_bytes, "%02x",out[ii]);
        strcat(dst, buf_bytes);
    }

    return 0;
}
