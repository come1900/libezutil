/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * str_opr.h - string opr func
 *
 * Copyright (C) 2005 - ezlibs.com -, All Rights Reserved.
 *
 * $Id: str_opr.h 10069 2007-07-12 07:36:25Z WuJunjie $
 *
 *  Explain:
 *     Some useful string function, find more yourself
 *
 *  Update:
 *     2005-03-23 17:19:24 WuJunjie 10221 Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef _STR_OPR_H
#define _STR_OPR_H

#include <stdio.h>
#include <time.h>

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * l_count_c - 
     * @ src : 
     *		data buf
     * @ __c : 
     *		the obj char
     *
     * the '__c' count in src from left
     */
    size_t l_count_c(char *src, const char __c);
    size_t r_count_c(char *src, const char __c);

    size_t l_trim_c(char *src, const char trimmer, size_t src_len);
    size_t r_trim_c(char *src, const char trimmer, size_t src_len);
    size_t trim_c(char *src, const char trimmer, size_t src_len);
    size_t trim_blank(char *src, size_t src_len);

    size_t replace_c(
        char *src,
        const char fromc,
        const char toc,
        size_t src_len);
    size_t replace_chars_with_char(
        char *strbuf,
        const char *chrstorm,
        const char chartorlcwith);
    // replace s1 with s2
    char* replace_chars_with_chars(char *s, const char *s1, const char *s2);

    /* danger func */
    size_t ini_str(char *src, const char __c, const  size_t src_size);

    int split_str(
        const char * const src,
        char *left,
        char *right,
        const char * const spliter,
        size_t src_len);
    int r_split_str(
        const char * const src,
        char *right,
        const char * const spliter,
        size_t src_len);
    // return strlen of dst, 0 - error or not found
    // get str between 2 markstring
    int m_split_str(
        const char * const src,
        char * const dst,
        const size_t dst_len,
        const char * const begin_spliter,
        const char * const end_spliter
    );

#define SHOW_HEXAD 0x01
	#define SHOW_ASCII 0x02
	#define SHOW_BINAR 0x04
	#define SHOW_LINER 0x08

    int dumpBuffer2Buffer(
        char *p_dst,
        const size_t i_dst_len,
        unsigned char *buf,
        const size_t len,
        size_t uiPrintMask);
    int dumpBuffer(
        FILE *fp,
        unsigned char *buf,
        const size_t len,
        size_t uiPrintMask);
    //		dumpBuffer(stdout
    //							, (U8 *)buff, bufflen,
    //			SHOW_ASCII | SHOW_BINAR | SHOW_HEXAD | SHOW_LINER);

    #define IP4_ADD_STR_LEN_MIN 7 /*0.0.0.0*/
    char * Ip2Str(const unsigned int iIp, char *pStr);
    unsigned int Str2Ip(const char *pStr);
    // 1 - private, 0- not
    int isPrivateIPAddress(char *pStr);
    /*
      return:
            1 - yes, 0 - no
    */
    int isCommentLine(char *src);
    int is_ascii_string(const char * const src, const size_t len);
    int is_isdigit_string(const char * const src, const size_t len);
#ifdef HAVE_ICONV
    int ez_gbk_to_utf8(char *toStr, size_t toStrSize, const char * gbkStr);
#endif //HAVE_ICONV

    #define GET_DATE_TIME_STRING_DEFAULT_STR_LEN 20
    #define def_EZ_DATE_TIME_STRING_FORMAT "%04d-%02d-%02d %02d:%02d:%02d"
    #define GET_DATE_STRING_DEFAULT_STR_LEN 20
    #define def_EZ_DATE_STRING_FORMAT "%04d-%02d-%02d"
    #define GET_TIME_STRING_DEFAULT_STR_LEN 20
    #define def_EZ_TIME_STRING_FORMAT "%02d:%02d:%02d"
    /**
     * char pBuff[32] or bigger;
     * if pFormat=NULL : "%04d-%02d-%02d %02d:%02d:%02d"
     */
    char * get_date_time_string(char *pBuff, const char *pFormat);
    // replaced by:get_date_time_string("%04d%02d%02d%02d%02d%02d");
    //char * get_date_time_string_compress();

    // convert from "Sep 27 2012 09:04:31" to struct tm,
    // use as int r = get_build_date_time(&__tm, __DATE__, __TIME__);
    int get_build_date_time(struct tm *p_tm
                            , const char * const p_date_str
                            , const char * const p_time_str);

    /**
     * time1: timt_t value
     * szTime: dsc string
     * if pFormat=NULL : "%04d-%02d-%02d %02d:%02d:%02d"
     */
    int date_time2string(const time_t time1, char *szTime, const char *pFormat);
    int date2string(const time_t time1, char *szTime, const char *pFormat);
    int time2string(const time_t time1, char *szTime, const char *pFormat);
    // szTime: 2012-11-02 13:33:17
    time_t string2date_time(char * szTime, const char *pFormat);

    unsigned long ez_bcd_2_dec(const unsigned char *bcd, int length);
    int ez_dec_2_bcd(int dec, unsigned char *bcd, int length);

    // replaced by: ez_parse_url
    int __ez_parse_http_url(const char *p_url, char *p_host, int *p_port);
    int __ez_prase_rtsp_url(const char *p_url, char *p_host, int *p_port);
    // 高效函数
    // 支持 "a=b"; " a=b"; "a b"; 等单字符分割的配置文件； 不支持关键字和分隔符分离的情况："a = b"
    int ez_get_ext_value(const char *pBuffer, const int iBufferLen, const char *pField, char *pValue, const int iValueLen);
    
    // string up low convert
    int ez_ascii_string_toupper(char *pString);
    int ez_ascii_string_tolower(char *pString);

    #define def_ez_uuid_len (sizeof("d7acd6d9-7321-4535-8c54-a572fee5216c")-1)
    // return NULL -- failed, others good
    // //if ui_seed ==0 , seed self
    char *ez_uuid(char *dst_uuid, const unsigned int ui_seed, const char * const p_seed);

    time_t time_iso8601_to_timet (const char *input);
    // gen rand data to gen_buf
    // return gen bytes, 0-failed
    size_t ez_rand_buf(char *gen_buf, size_t gen_size);
    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifdef __cplusplus
}
#endif

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#endif //_STR_OPR_H
