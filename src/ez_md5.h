/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ez_md5.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ez_md5.h 5884 2017-05-18 05:06:00Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2017-05-18 05:06:00  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef _ez_md5_H
#define _ez_md5_H

#ifdef __cplusplus
extern "C"
{
#endif


#define EZ_STD_MD5_RESULT_LEN 16 // Byte
    inline int ez_cal_std_md5(char *dst, size_t dst_len/*16*/, const char *src, size_t src_len);

    //转换为大写字符串的字符长度， 缓存长度注意+1
#define EZ_STD_MD5_STRING_LEN_UP_32B 32 // Byte

    int ez_cal_std_md5_32b_up_string(char *dst, size_t dst_len/*33*/, const char *src, size_t src_len);
    int ez_cal_std_md5_32b_low_string(char *dst, size_t dst_len/*33*/, const char *src, size_t src_len);

#ifdef __cplusplus
}
#endif

#endif //_ez_md5_H

