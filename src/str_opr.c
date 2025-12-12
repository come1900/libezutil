/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * str_opr.c - string opr func
 *
 * Copyright (C) 2005 ezlibs.com, All Rights Reserved.
 *
 * $Id: str_opr.c 10192 2007-07-18 00:43:21Z WuJunjie $
 *
 *  Explain:
 *     Some useful string function, find more yourself.
 *
 *  Update:
 *     2005-03-23 17:19:24 WuJunjie 10221 Create
 * 
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#ifdef HAVE_ICONV
#include "iconv.h"
#endif //HAVE_ICONV

#include "str_opr.h"

#ifdef _MSC_VER
#pragma warning(disable:4786) // long name warning
#pragma warning(disable: 4996)  // secure version deprecation warnings
//#pragma warning(disable: 4267)  // 64 bit signed/unsigned loss of data
#endif

#ifdef _DEBUG
        #define DEB(x) x
        #define DBG(x) x
#else
        #define DEB(x)
        #define DBG(x)
#endif

#ifndef __trip
    #define __trip
//#define __trip printf("-W-%d::%s(%d)\n", (int)time(NULL), __FILE__, __LINE__);
#endif
#ifndef __fline
    #define __fline
//#define __fline printf("%s(%d)--", __FILE__, __LINE__);
#endif

#define ARG_USED(x) (void)&x;

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

size_t l_count_c(char *src, const char __c)
{

    char *p;

    if (!src || !*src || !__c)
        return 0;

    p = src;

    while ((*p != '\0')
           && (*p == __c))
    {
        p++;
    }

    return (size_t)(p-src);
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

size_t r_count_c(char *src, const char __c)
{
    char *p;
    char *p_tail;

    if (!src || !*src || !__c)
        return 0;

    p_tail = src + strlen(src)-1;
    p=p_tail;

    while ( (p>=src)
            && (*p == __c) )
    {
        p--;
    }
    return (size_t)(p_tail-p);
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/**
 * l_trim_c - 
 * src:
 * trimmer: 
 * src_len:
 * Desc
 */
size_t l_trim_c(char *src, const char trimmer, size_t src_len)
{
    size_t pass_len;
    size_t real_len;

    /* check input*/
    if (!src || !*src || !trimmer)
        return 0;

    real_len = strlen(src);
    src_len = src_len==0 ? real_len : src_len;
    src_len = src_len>real_len ? real_len : src_len;

    pass_len = l_count_c(src, trimmer);

    if (pass_len>0)
    {
        src_len -= pass_len;
        memmove(src, src+pass_len, src_len);
        src[src_len] = '\0';
    }

    return (size_t)(pass_len);
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

size_t r_trim_c(char *src, const char trimmer, size_t src_len)
{
    char *p_tail;
    size_t pass_len = 0;
    size_t real_len;

    /* check input*/
    if (!src || !*src || !trimmer)
        return 0;

    real_len = strlen(src);
    src_len = src_len==0 ? real_len : src_len;
    src_len = src_len>real_len ? real_len : src_len;

    p_tail = src + src_len -1;

    while ( (p_tail>=src)
            && (*p_tail==trimmer) )
    {
        *p_tail-- = '\0';
        pass_len++;
    }

    /* get right part */
    if (src_len<real_len)
        *(src+src_len) = '\0';

    return (size_t)(pass_len);
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

size_t trim_c(char *src, const char trimmer, size_t src_len)
{
    size_t pass_len;

    pass_len  = l_trim_c(src, trimmer, src_len);
    pass_len += r_trim_c(src, trimmer, src_len);

    return pass_len;
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

size_t trim_blank(char *src, size_t src_len)
{
    size_t pass_len;
    size_t real_len;
    char *p = src;

    /* check input*/
    if (!src || !*src)
        return 0;

    real_len = strlen(src);
    src_len = src_len==0 ? real_len : src_len;
    src_len = src_len>real_len ? real_len : src_len;

    while ((*p != '\0')
           && ( (*p == ' ')
                || (*p == '\t')
                || (*p == '\r')
                || (*p == '\n') ) )
    {
        p++;
    }

    pass_len = p-src;
    src_len = src_len-pass_len;

    memmove(src, p, src_len);
    src[src_len] = '\0';

    p = src + src_len - 1;
    while (p >= src)
    {
        if ( (*p == ' ')
             || (*p == '\t')
             || (*p == '\r')
             || (*p == '\n') )
        {
            *p-- = 0;
            pass_len++;
        }
        else
        {
            break;
        }
    }

    return (size_t)(pass_len);
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

size_t replace_c(
    char *src,
    const char fromc,
    const char toc,
    size_t src_len)
{
    size_t real_len;
    size_t num;
    char *p;

    p = src;
    num = 0;

    /* check input*/
    if (!src || !*src || !fromc || !toc)
        return 0;

    real_len = strlen(src);
    src_len = src_len==0 ? real_len : src_len;
    src_len = src_len>real_len ? real_len : src_len;

    while (*p != '\0')
    {
        if (*p == fromc)
        {
            *p = toc;
            num++;
        }

        p++;
    }

    return (size_t)(num);
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

size_t replace_chars_with_char(
    char *strbuf,
    const char *chrstorm,
    const char chartorlcwith)
{
    char *offset;
    size_t replace_chars;

    /* check input*/
    if (!strbuf || !*strbuf || !chrstorm || !*chrstorm)
        return 0;

    offset = (char *)NULL;
    replace_chars = 0;

    while (*strbuf)
    {
        offset = strpbrk (strbuf, chrstorm);
        if (offset)
        {
            *(offset) = chartorlcwith;
            replace_chars++;
        }
        else
        {
            break;
        }
    }

    return replace_chars;
}

// s1 will be replaced by s2 in s
char* replace_chars_with_chars(char *s, const char *s1, const char *s2)
{
    char *ptr = s;

    if ( s==NULL || *s=='\0'
         || s1==NULL || *s1=='\0'
         || s2==NULL || *s2=='\0'
       )
    {
        return NULL;
    }

    while ( NULL != (ptr = strstr(ptr, s1)) )
    {
        memmove(ptr + strlen(s2) , ptr + strlen(s1), strlen(ptr) - strlen(s1) + 1);
        memcpy(ptr, s2, strlen(s2));

        // 防止s2中还有s1的情况， 这样会重复替换
        ptr += strlen(s2);
    }

    return s;
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/**
 * ini_str - 
 * @: 
 * @: 
 *
 * danger func
 */
size_t ini_str(char *src, const char __c, const  size_t src_size)
{
    /* check input*/
    if (!src || !*src)
        return 0;
    memset(src+src_size-1, 0, 1);
    memset(src, __c, src_size-1);
    return (size_t)src_size;
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/**
 * split_str - 
 * @: 
 * @: 
 *
 * find func based strstr, it find from left
 */
int split_str(
    const char * const src,
    char *left,
    char *right,
    const char * const spliter,
    size_t src_len)
{
    char *p;
    size_t real_len;

    /* check input*/
    if (!src || !*src || !left || !right)
    {
        return -1000;
    }

    real_len = strlen(src);
    src_len = src_len==0 ? real_len : src_len;
    src_len = src_len>real_len ? real_len : src_len;

    strncpy(right, src, src_len);
    right[src_len] = '\0';

    /* get right pos */
    p = strstr(right, spliter);
    /* not found spliter */
    if (!p)
    {
#if 0
        // ret error here
        *left = '\0';
        *right = '\0';
        return -1;
#else
        // we load src to lefp
        // ret ok here
        strcpy(left, src);
        *right = '\0';
        return 0;
#endif

    }
    /* left */
    strncpy(left, right, p-right);
    left[p-right] = '\0';
    /* right */
    strcpy(right, p+strlen(spliter));

    return 0;
}


/**
 * func_name - 
 * @: 
 * @: 
 *
 * 分隔字符串， 取分隔符后面(右边)的值
 */
int r_split_str(
    const char * const src,
    char *right,
    const char * const spliter,
    size_t src_len)
{
    char *p;
    size_t real_len;

    /* check input*/
    if (!src || !*src || !right)
        return -1000;
    real_len = strlen(src);
    src_len = src_len==0 ? real_len : src_len;
    src_len = src_len>real_len ? real_len : src_len;

    strncpy(right, src, src_len);
    right[src_len] = '\0';

    /* get right pos */
    p = strstr(right, spliter);
    /* not found spliter */
    if (!p)
    {
        *right = '\0';
        return -1;
    }
    /* right */
    strcpy(right, p+strlen(spliter));

    return 0;
}
/**
 * func_name - 
 * @: 
 * @: 
 *
 * get str between 2 markstring
 * return strlen of dst, 0 - error or not found
 */
int m_split_str(
    const char * const src,
    char * const dst,
    const size_t dst_len,
    const char * const begin_spliter,
    const char * const end_spliter
)
{
    int iret = 0;

    if (!src
        || '\0'==*src
        || !dst
        || dst_len<1
        || !begin_spliter
        || '\0'==*begin_spliter
        || !end_spliter
        || '\0'==*end_spliter)
    {
        //__trip;
        return 0;
    }

    size_t real_len_src = strlen(src);
    size_t real_len_begin_spliter = strlen(begin_spliter);
    size_t real_len_end_spliter = strlen(end_spliter);

    //
    if (real_len_src<=(real_len_begin_spliter+real_len_end_spliter))
    {
        //__trip;
        return 0;
    }

    //printf("  ms_response.size:%ld\n", real_len_src);
    //printf("ms_response.memory:\n[%s]\n", src);

    do
    {

        //        const char *pMarkStart="<LocalPlayUrl>";
        //        const int lenMarkStart = strlen(pMarkStart);
        //        const char *pMarkEnd="</LocalPlayUrl>";

        char *startP=strstr(src, begin_spliter);
        if (NULL == startP)
        {
            __trip;
            //iret = -100;
            break;
        }

        char *endP=strstr(startP+real_len_begin_spliter, end_spliter);
        if (NULL == endP)
        {
            __trip;
            //iret = -100;
            break;
        }
        // 地址长度检查
        //        if (endP<(startP+real_len_begin_spliter+4/*rtsp://*/))
        //        {
        //            __trip;
        //            iret = -100;
        //            break;
        //        }

        //地址长度检查
        if (endP-(startP+real_len_begin_spliter) > dst_len)
        {
            __trip;
            //iret = -100;
            break;
        }

        memcpy(dst, startP+real_len_begin_spliter, endP-(startP+real_len_begin_spliter) );
        iret = endP-(startP+real_len_begin_spliter);
    }
    while (0);

    return iret;
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
// Return TotalLine
int dumpBuffer2Buffer(
    char *p_dst,
    const size_t i_dst_len,
    unsigned char *buf,
    const size_t len,
    size_t uiPrintMask)
{
    unsigned int iTotalLine;
    unsigned int iLiner;
    unsigned int iPostion;
    unsigned int iSize = 0; // 当前长度
    char tmp_buff[16];
    //int i_ret = -100;

    p_dst[0] = '\0';

    if (0 == uiPrintMask)
        return 0;

    if (0 == len)
        return 0;

    iTotalLine = len/16 + ((len%16) ? 1 : 0);

    for (iLiner=1; iLiner<=iTotalLine; iLiner++)
    {
        if (uiPrintMask & SHOW_LINER)
        {
            //fprintf(fp, "%04d: ", iLiner);
            iSize += sprintf(tmp_buff, "%04d: ", iLiner);
            if (iSize>i_dst_len-1)
                return -100;

            strcat(p_dst, tmp_buff);
        }

        if (uiPrintMask & SHOW_HEXAD)
        {
            //fprintf(fp, "0x%06x: ", 16*(iLiner-1));
            iSize += sprintf(tmp_buff, "0x%06x: ", 16*(iLiner-1));
            if (iSize>i_dst_len-1)
                return -100-1;

            strcat(p_dst, tmp_buff);
        }

        if (uiPrintMask & SHOW_BINAR)
            for (iPostion=16*(iLiner-1); (iPostion<16*(iLiner-1)+16); iPostion++)
            {
                if (iPostion<len)
                {
                    //fprintf(fp, "%02x ", buf[iPostion]);
                    iSize += sprintf(tmp_buff, "%02x ", buf[iPostion]);
                    if (iSize>i_dst_len-1)
                        return -100-2;

                    strcat(p_dst, tmp_buff);
                }
                else
                {
                    //fprintf(fp, "   ");
                    iSize += sprintf(tmp_buff, "   ");
                    if (iSize>i_dst_len-1)
                        return -100-3;

                    strcat(p_dst, tmp_buff);
                }
            }

        if (uiPrintMask & SHOW_ASCII)
        {
            //fprintf(fp, "; ");
            iSize += sprintf(tmp_buff, "; ");
            if (iSize>i_dst_len-1)
                return -100-4;

            strcat(p_dst, tmp_buff);

            for (iPostion=16*(iLiner-1); (iPostion<len) && (iPostion<16*(iLiner-1)+16); iPostion++)
            {
                if ( isprint(buf[iPostion]) )
                {
                    //fprintf(fp, "%c", buf[iPostion]);
                    iSize += sprintf(tmp_buff, "%c", buf[iPostion]);
                    if (iSize>i_dst_len-1)
                        return -100-5;

                    strcat(p_dst, tmp_buff);
                }
                else
                {
                    //fprintf(fp, "%c", '.');
                    iSize += sprintf(tmp_buff, "%c", '.');
                    if (iSize>i_dst_len-1)
                        return -100-6;

                    strcat(p_dst, tmp_buff);
                }
            }
        }
        //fprintf(fp, "\n");
        iSize += sprintf(tmp_buff, "\n");
        if (iSize>i_dst_len-1)
            return -100-7;

        strcat(p_dst, tmp_buff);
    }

    return iTotalLine;
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
// Return TotalLine
int dumpBuffer(FILE *fp,
               unsigned char *buf,
               const size_t len,
               size_t uiPrintMask)
{
    unsigned int iTotalLine;
    unsigned int iLiner;
    unsigned int iPostion;

    if (0 == uiPrintMask)
        return 0;

    if (0 == len)
        return 0;

    iTotalLine = len/16 + ((len%16) ? 1 : 0);

    for (iLiner=1; iLiner<=iTotalLine; iLiner++)
    {
        if (uiPrintMask & SHOW_LINER)
            fprintf(fp, "%04d: ", iLiner);

        if (uiPrintMask & SHOW_HEXAD)
            fprintf(fp, "0x%06x: ", 16*(iLiner-1));

        if (uiPrintMask & SHOW_BINAR)
            for (iPostion=16*(iLiner-1); (iPostion<16*(iLiner-1)+16); iPostion++)
            {
                if (iPostion<len)
                    fprintf(fp, "%02x ", buf[iPostion]);
                else
                    fprintf(fp, "   ");
            }

        if (uiPrintMask & SHOW_ASCII)
        {
            fprintf(fp, "; ");
            for (iPostion=16*(iLiner-1); (iPostion<len) && (iPostion<16*(iLiner-1)+16); iPostion++)
            {
                if ( isprint(buf[iPostion]) )
                {
                    fprintf(fp, "%c", buf[iPostion]);
                }
                else
                {
                    fprintf(fp, "%c", '.');
                }
            }
        }
        fprintf(fp, "\n");
    }

    return iTotalLine;
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * following use LITTLE_ENDIAN memory
 */
char * Ip2Str(const unsigned int iIp, char *pStr)
{
    unsigned char i;
    unsigned char cIp[4];

    for (i=0; i<4; i++)
        cIp[i] = (unsigned char)( (iIp>>(i*8)) & (0xff) );

    sprintf(pStr, "%d.%d.%d.%d", cIp[0], cIp[1], cIp[2], cIp[3]);
    return pStr;
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/**
 *  - 
 * @ : 
 * @ :
 * Desc
 *	j >= 0 used when BIG_ENDIAN
 *	do not comment it
 */
unsigned int Str2Ip(const char *pStr)
{
    signed char j = 0;
    unsigned char cIp;
    unsigned int uiRet = 0;
    const char *p = pStr;

    cIp = (unsigned char)atoi(p);
    uiRet |= ( ((unsigned int)(cIp)) << (j*8) );
    j++;

    while ( (*p != 0)
            && j >= 0)
    {
        if (*p != '.')
        {
            p++;
            continue;
        }

        cIp = (unsigned char)atoi(++p);
        uiRet |= ( ((unsigned int)(cIp)) << (j*8) );
        j++;

    }

    return uiRet;
}

#ifdef LINUX
#include <arpa/inet.h>
#endif

int isPrivateIPAddress(char *pStr)
{
    unsigned int ip;
    unsigned char b1, b2;//, b3, b4;

    ip = Str2Ip(pStr);

    ip = ntohl(ip);

    b1 = (unsigned char )(ip >> 24);
    b2 = (unsigned char )((ip >> 16) & 0x0ff);
    //b3 = (unsigned char )((ip >> 8) & 0x0ff);
    //b4 = (unsigned char )(ip & 0x0ff);

    // 10.x.y.z
    if (b1 == 10)
        return 1;

    // 172.16.0.0 - 172.31.255.255
    if ((b1 == 172) && (b2 >= 16) && (b2 <= 31))
        return 1;

    // 192.168.0.0 - 192.168.255.255
    if ((b1 == 192) && (b2 == 168))
        return 1;

   // 1-126 A ip, only 58 in china
    if (b1>=1 && b1<=126 && b1!=58)
        return 1;

    return 0;
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

int isCommentLine(char *src)
{
    char *p = src;

    /* check input*/
    if (!src || !*src)
        return 1;

    // skip blank
    while ((*p != '\0')
           && ( (*p == ' ')
                || (*p == '\t')
                || (*p == '\r')
                || (*p == '\n') ) )
    {
        p++;
    }

    // ‘#’ is commit charactor
    if (*p == '#' || *p == '\0')
    {
        return 1;
    }

    return 0;
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

int is_ascii_string(const char * const src, const size_t len)
{
    size_t ii;

    for (ii=0; ii<len; ii++)
    {
        if ('\0' == src[ii])
        {
            break;
        }

        if (!isascii(src[ii]))
        {
            return 0;
        }
    }

    return 1;
}
int is_isdigit_string(const char * const src, const size_t len)
{
    size_t ii;

    for (ii=0; ii<len; ii++)
    {
        if ('\0' == src[ii])
        {
            break;
        }

        if (!isdigit(src[ii]))
        {
            return 0;
        }
    }

    return 1;
}
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifdef HAVE_ICONV
int __ez_iconv(char *toStr, size_t toStrSize, const char * fromStr, const char* toCode, const char* fromCode);

int __ez_iconv(char *toStr, size_t toStrSize, const char * fromStr, const char* toCode, const char* fromCode)
{
    iconv_t iconvH;
    char* pBuff;
    char* strChar;
    char** pin;
    char* outbuf;
    size_t strLength;
    size_t outLength;

    iconvH = iconv_open(fromCode, toCode);

    if (iconvH == 0)
    {
        return -1;
    }

    strChar = (char *)fromStr;

    pin = (char**)&strChar;

    strLength = strlen(fromStr);

    //char* outbuf = (char*) malloc(strLength*4);
    outbuf = toStr;
    // 缓存过小， gbk 2 utf8 使用*4
    if (toStrSize < strLength*4 + 4)
    {
        return -1;
    }

    pBuff = outbuf;

    memset( outbuf, 0, strLength*4);
    outLength = strLength*4;

    if (-1 == iconv(iconvH, pin, &strLength, &outbuf, &outLength))
    {
        iconv_close(iconvH);
        return -2;
    }

    //gbkStr = pBuff;
    toStr =  pBuff;

    iconv_close(iconvH);

    return 0;
}

int ez_gbk_to_utf8(char *toStr, size_t toStrSize, const char * gbkStr)
{
    return __ez_iconv(toStr, toStrSize, gbkStr, "gb2312", "utf-8");
}

#endif //HAVE_ICONV

#include <time.h>

char * get_date_time_string(char *pBuff, const char *pFormat)
{
    //static char datetimestr[32];
    struct tm *tm_now;
    time_t tt_now;

    tt_now = time(NULL);
    tm_now = localtime(&tt_now);

    if (pBuff==NULL)
    {
        return NULL;
    }

    if (pFormat==NULL)
    {
        pFormat = "%04d-%02d-%02d %02d:%02d:%02d";
    }

    sprintf(pBuff, pFormat,
            tm_now->tm_year+1900,
            tm_now->tm_mon+1,
            tm_now->tm_mday,
            tm_now->tm_hour,
            tm_now->tm_min,
            tm_now->tm_sec);

    return pBuff;
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

//char * get_date_time_string_compress()
//{
//    static char datetimestr[32];
//
//    struct tm *tm_now;
//    time_t tt_now;
//    tt_now = time(NULL);
//    tm_now = localtime(&tt_now);
//
//    sprintf(datetimestr, "%04d%02d%02d%02d%02d%02d",
//            tm_now->tm_year+1900,
//            tm_now->tm_mon+1,
//            tm_now->tm_mday,
//            tm_now->tm_hour,
//            tm_now->tm_min,
//            tm_now->tm_sec);
//
//    return datetimestr;
//}

int get_build_date_time(struct tm *p_tm, const char * const p_date_str, const char * const p_time_str)
{
    int i;
    char mon_str[4];
    char* __month_all[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    //char *wday[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

    sscanf(p_date_str, "%s %d %d", mon_str, &p_tm->tm_mday, &p_tm->tm_year);

    for (i = 0; i < 12; i++)
    {
        if( strncmp(__month_all[i], p_date_str, 3) == 0 )
        {
            break;
        }
    }
    p_tm->tm_mon = i+1;

    sscanf(p_time_str, "%d:%d:%d", &p_tm->tm_hour, &p_tm->tm_min, &p_tm->tm_sec);

    if (p_tm->tm_mon>12
        || p_tm->tm_mday>31
        || p_tm->tm_year<1970)
    {

        return -1;
    }

    return 0;
}

int date2string(const time_t time1, char *szTime, const char *pFormat)
{
    struct tm tm1;
    if (pFormat==NULL)
    {
        pFormat = def_EZ_DATE_STRING_FORMAT;//"%04d-%02d-%02d %02d:%02d:%02d";
    }

    if (szTime==NULL)
    {
        return -1;
    }


#ifdef WIN32
    tm1 = *localtime(&time1);
#else

    localtime_r(&time1, &tm1 );
#endif

    sprintf( szTime, pFormat,
             tm1.tm_year+1900, tm1.tm_mon+1, tm1.tm_mday);

    return 0;
}

int time2string(const time_t time1, char *szTime, const char *pFormat)
{
    struct tm tm1;
    if (pFormat==NULL)
    {
        pFormat = def_EZ_TIME_STRING_FORMAT;//"%04d-%02d-%02d %02d:%02d:%02d";
    }

    if (szTime==NULL)
    {
        return -1;
    }


#ifdef WIN32
    tm1 = *localtime(&time1);
#else

    localtime_r(&time1, &tm1 );
#endif

    sprintf( szTime, pFormat,
             tm1.tm_hour, tm1.tm_min,tm1.tm_sec);

    return 0;
}

int date_time2string(const time_t time1, char *szTime, const char *pFormat)
{
    struct tm tm1;
    if (pFormat==NULL)
    {
        pFormat = "%04d-%02d-%02d %02d:%02d:%02d";
    }

    if (szTime==NULL)
    {
        return -1;
    }


#ifdef WIN32
    tm1 = *localtime(&time1);
#else

    localtime_r(&time1, &tm1 );
#endif

    sprintf( szTime, pFormat,
             tm1.tm_year+1900, tm1.tm_mon+1, tm1.tm_mday,
             tm1.tm_hour, tm1.tm_min,tm1.tm_sec);

    return 0;
}

//szTime fmt: 2012-11-02 11:12:48
time_t string2date_time(char * szTime, const char *pFormat)
{
    if (pFormat==NULL)
    {
        pFormat = "%04d-%02d-%02d %02d:%02d:%02d";
    }

    if (szTime==NULL)
    {
        return -1;
    }

    struct tm tm1;
    time_t time1;

    //sscanf(szTime, "%4d-%2d-%2d %2d:%2d:%2d",
    sscanf(szTime, pFormat,
           &tm1.tm_year,
           &tm1.tm_mon,
           &tm1.tm_mday,
           &tm1.tm_hour,
           &tm1.tm_min,
           &tm1.tm_sec);
    tm1.tm_year -= 1900;
    tm1.tm_mon --;
    tm1.tm_isdst=-1;
    time1 = mktime(&tm1);

    return time1;
}

/////////////////////////////////////////////////////////
//
//Function: BCD to dec (10)
//
//Input: const unsigned char *bcd     BCD
//       int length                   len of BCD data
//
//OutPut:
//
//Return: unsigned long
//
//Memo: 
//////////////////////////////////////////////////////////
unsigned long ez_bcd_2_dec(const unsigned char *bcd, int length)
{
    int i, tmp;
    unsigned long dec = 0;
    for (i = 0; i < length; i++)
    {
        tmp = ((bcd[i] >> 4) & 0x0F) * 10 + (bcd[i] & 0x0F);
        dec += (unsigned long)(tmp * pow((double)100, length - 1 - i));
    }
    return dec;
}

int ez_dec_2_bcd(int Dec, unsigned char *Bcd, int length)
{
    int i;
    int temp;
    for (i = length - 1; i >= 0; i--)
    {
        temp = Dec % 100;
        Bcd[i] = ((temp / 10) << 4) + ((temp % 10) & 0x0F);
        Dec /= 100;
    }

    return 0;
}


int __ez_parse_http_url(const char *p_url, char *p_host, int *p_port)
{
    if (p_url == NULL || p_url[0]=='\0')
    {
        return -1;
    }

    //int url_len = strlen(p_url);

    char *p_str_url = (char *)p_url;
    char *p_pos_begin_host = NULL;
    char *p_pos_end_host = NULL;
    char *p_pos_begin_port = NULL;

    p_pos_begin_host = strstr(p_str_url, "http://");
    if (p_pos_begin_host == NULL)
    {
        p_pos_begin_host = strstr(p_str_url, "https://");
        if (p_pos_begin_host == NULL)
        {
            p_pos_begin_host = p_str_url;
        }
        else
        {
            p_pos_begin_host+=strlen("https://");
        }
    }
    else
    {
        p_pos_begin_host+=strlen("http://");
    }

    p_pos_end_host = strstr(p_pos_begin_host, ":");
    if (p_pos_end_host==NULL)
    {
        p_pos_end_host = strstr(p_pos_begin_host, "/");
        if (p_pos_end_host==NULL)
        {
            p_pos_end_host = p_pos_begin_host+strlen(p_pos_begin_host);
        }
    }
    else
    {
        p_pos_begin_port=p_pos_end_host+1;
    }

    int __host_size = (p_pos_end_host-p_pos_begin_host);
    memcpy(p_host, p_pos_begin_host, __host_size);
    p_host[__host_size] = '\0';
    if (!isdigit(p_host[0]))
    {
        p_host[0] = '\0';
        *p_port = 0;
        return -1;
    }


    if (p_pos_begin_port!=NULL)
    {
        *p_port = atoi(p_pos_begin_port);
    }
    else
    {
        *p_port = 80;
    }

    return 0;
}

#ifdef LINUX
#define strnicmp strncasecmp
#endif

#ifndef __trip
	#define __trip

//#define __trip printf("-W-%d::%s(%d)\n", (int)time(NULL), __FILE__, __LINE__);
#endif
#ifndef __fline
	#define __fline printf("%s(%d)--", __FILE__, __LINE__);
#endif

int __ez_prase_rtsp_url( const char *p_url, char *p_host, int *p_port )
{
    int lret = -1;

    __trip
    if(p_url!=NULL && *p_url!='\0')
    {
        __trip
        //1.先找到rtsp这个协议字段
        if(!strnicmp(p_url, "rtsp://", 7))
        {
            __trip
            //2.再找ip地址
            char *s, *ss;
            s = (char*)p_url + strlen("rtsp://");
            ss = strchr(s, '/');

            strncpy(p_host, s,  ss- s);
            p_host[ss -s] = '\0';

            //3.查找下是否是有端口设置
            s = strchr(p_host, ':');
            //如果有端口设置，还需要修改IP地址
            if(s)
            {
                __trip
                ss = s;
                s ++;
                *p_port = atoi(s);

                //同时修正ip地址
                p_host[ss - p_host] = '\0';
            }
            else
            {
                __trip
                *p_port = 554;//default 554
            }
            __trip

            lret = 0;
        }
    }

    __trip
    return lret;
}

// 高效函数
// 支持 "a=b"; " a=b"; "a b"; 等单字符分割的配置文件； 不支持关键字和分隔符分离的情况："a = b"
int ez_get_ext_value(const char *pBuffer, const int iBufferLen, const char *pField, char *pValue, const int iValueLen)
{
    char *pFieldBegin = NULL;
    char *pFieldEnd = NULL;
    int iFieldLen;
    int iRet = -1;

    if (pBuffer==NULL
        || pField==NULL
        || pValue==NULL)
    {
        return -2;
    }

    pFieldBegin = strstr((char *)pBuffer, pField);
    if (pFieldBegin)
    {
        pFieldEnd = strstr(pFieldBegin, "\n");
        if (pFieldEnd==NULL)
            pFieldEnd = (char *)(pBuffer+iBufferLen);
        //pFieldEnd = strstr(pFieldBegin, "\0");
    }

    if (pFieldEnd)
    {
        iFieldLen = strlen(pField)+1;// +1 for :

        if (pFieldEnd-pFieldBegin>iFieldLen)
        {
            // find, len iret
            iRet = pFieldEnd-pFieldBegin-iFieldLen;
            // value buffer not enough
            if (iRet>=iValueLen)
            {
                return -3;
            }

            // trim /r only, blank reserved
            if ( iRet>0 && *(pFieldBegin+iFieldLen+iRet-1) == '\r' )
            {
                iRet--;
            }

            strncpy(pValue, pFieldBegin+iFieldLen, iRet);
        }
    }

    return iRet;
}

int ez_ascii_string_toupper(char *pString)
{
    char *p = pString;

    if (!p || !*p)
    {
        return -1;
    }

    while(*p)
    {
        *p = toupper(*p);

        ++p;
    }

    return p-pString;
}

int ez_ascii_string_tolower(char *pString)
{
    char *p = pString;

    if (!p || !*p)
    {
        return -1;
    }

    while(*p)
    {
        *p = tolower(*p);

        ++p;
    }

    return p-pString;
}

#define def_rand_buf_size 256//64+64
#include "sha1.h"

#if !defined(_WIN32)
#include <sys/time.h>
#endif//gettimeofday

//if ui_seed ==0 , seed self
char *ez_uuid(char *dst_uuid, const unsigned int ui_seed, const char * const p_seed)
{
    static char __ez_uuid_buf[def_ez_uuid_len+1] = {'\0'};
    static unsigned int __seedFix=1;
    char *ez_uuid_buf = dst_uuid==NULL?__ez_uuid_buf:dst_uuid;
    const char *__p_seed = p_seed==NULL ? "472325191" : p_seed;

    //新申请内存， 但是不初始化， 使用随机数据
    char *d_buf = (char *)malloc(def_rand_buf_size);
    if (NULL == d_buf)
    {
        return NULL;
    }

    __seedFix++;
    unsigned int seed = ui_seed;

    if (ui_seed==0)
    {
#if defined(_WIN32)
        seed = time(NULL) + __seedFix;
        //#elif defined(LINUX) || defined(MACOSX) || defined(IOS)
#else
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME_COARSE, &ts);

        seed = ts.tv_sec + ts.tv_nsec + __seedFix;
#endif// linux

    }

    srand (seed);

    snprintf(d_buf, def_rand_buf_size, "%de%dz%dl%dibs.com%d-%s"
             , rand()%10, rand()%10, rand()%10, rand()%10, rand()%10, __p_seed);

    char dst[SHA1HashSizeDestBuffer] = {0};
    //const char *src = d_buf;

    //int iret = ez_sha1_str(dst, sizeof(dst), src );
    int iret = ez_sha1(dst, sizeof(dst), d_buf , def_rand_buf_size);
    //printf("src:%s, dst:%s\n", src, dst);

    if (iret<0)
    {
        strncpy(ez_uuid_buf, d_buf, def_ez_uuid_len);
    }
    else
    {
        strncpy(ez_uuid_buf, dst, def_ez_uuid_len);
        //printf("src:%s, dst:%s\n", src, dst);
        ez_uuid_buf[8] = '-';
        ez_uuid_buf[13] = '-';
        ez_uuid_buf[18] = '-';
        ez_uuid_buf[23] = '-';
    }

    free (d_buf);

    return ez_uuid_buf;
}

time_t
time_iso8601_to_timet (const char *input)
{
    if (input == NULL
        || *input=='\0'
       )
    {
        return -1;
    }

    int iret = 0;
    struct tm tm_info;
    memset(&tm_info, 0, sizeof(struct tm));

    iret = sscanf(input, "%d-%d-%dT%d:%d:%d"
                  , &tm_info.tm_year
                  , &tm_info.tm_mon
                  , &tm_info.tm_mday
                  , &tm_info.tm_hour
                  , &tm_info.tm_min
                  , &tm_info.tm_sec
                 );

    if (iret!=6
        || tm_info.tm_year < 1900
        || tm_info.tm_mon < 1)
    {
        return -100;
    }
    //
    tm_info.tm_year-=1900;
    tm_info.tm_mon-=1;

    //printf("iret:%d\n", iret);

    return mktime(&tm_info);
}

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
// #define _defRandDevice "/dev/random" // 熵用完停
#define _defRandDevice "/dev/urandom" // 熵用完不停
size_t ez_rand_buf(char *gen_buf, size_t gen_size)
{
    if (NULL==gen_buf || gen_size<1)
        return 0;

    int fd = open(_defRandDevice, O_RDONLY);
    if (fd < 0)
        return 0;

    ssize_t result = read(fd, gen_buf, gen_size);

    close(fd);

    return result;
}
