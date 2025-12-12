/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * md5.cpp - _explain_
 *
 * $Id: md5.cpp 16262 2008-04-22 07:48:14Z wujunjie $
 *
 * add this file to encode the passwd string.
 * I add ez_md5_calc_8ascii() func to fit this requirement
 * you can use it nimbly, such as license etc. I know that
 * some telcom software use it to manage their license,
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2005-09-221 17:19:24 WuJunjie 10221 Create
 */
/*
 * Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
 * rights reserved.
 *
 * License to copy and use this software is granted provided that it
 * is identified as the "RSA Data Security, Inc. MD5 Message-Digest
 * Algorithm" in all material mentioning or referencing this software
 * or this function.
 *
 * License is also granted to make and use derivative works provided
 * that such works are identified as "derived from the RSA Data
 * Security, Inc. MD5 Message-Digest Algorithm" in all material
 * mentioning or referencing the derived work.
 *
 * RSA Data Security, Inc. makes no representations concerning either
 * the merchantability of this software or the suitability of this
 * software for any particular purpose. It is provided "as is"
 * without express or implied warranty of any kind.
 *
 * These notices must be retained in any copies of any part of this
 * documentation and/or software.
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include "md5.h"

/* Constants for MD5Transform routine.
 */
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

static void MD5Transform(unsigned long  aa[4], unsigned char bb[64]);
static void Encode(unsigned char *aa, unsigned long *bb, unsigned int cc);
static void Decode(unsigned long *aa, unsigned char *bb, unsigned int cc);
//static void MD5_memcpy(unsigned char * aa, unsigned char * bb, unsigned int cc);
//static void MD5_memset(unsigned char * aa, int bb, unsigned int cc);

static unsigned char PADDING[64] =
    {
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

/* F, G, H and I are basic MD5 functions.
 */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits.
 */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
Rotation is separate from addition to prevent recomputation.
 */
#define FF(a, b, c, d, x, s, ac) { \
    (a) += F ((b), (c), (d)) + (x) + (unsigned long)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}

#define GG(a, b, c, d, x, s, ac) { \
    (a) += G ((b), (c), (d)) + (x) + (unsigned long)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}

#define HH(a, b, c, d, x, s, ac) { \
    (a) += H ((b), (c), (d)) + (x) + (unsigned long)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}

#define II(a, b, c, d, x, s, ac) { \
    (a) += I ((b), (c), (d)) + (x) + (unsigned long)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}


/* MD5 initialization. Begins an MD5 operation, writing a new context.
 */
void ez_MD5Init(__MD5_CTX *context)
{
	context->count[0] = context->count[1] = 0;

	/* Load magic initialization constants.
	*/
	context->state[0] = 0x67452301;
	context->state[1] = 0xefcdab89;
	context->state[2] = 0x98badcfe;
	context->state[3] = 0x10325476;
}

/* MD5 block update operation. Continues an MD5 message-digest
  operation, processing another message block, and updating the
  context.
 */
void ez_MD5Update(__MD5_CTX *context, unsigned char *input, unsigned int inputLen)
{
	unsigned int i, index, partLen;

	/* Compute number of bytes mod 64 */
	index = (unsigned int)((context->count[0] >> 3) & 0x3F);

	/* Update number of bits */
	if ((context->count[0] += ((unsigned long)inputLen << 3))
	    < ((unsigned long)inputLen << 3))
	{
		context->count[1]++;
	}
	context->count[1] += ((unsigned long)inputLen >> 29);

	partLen = 64 - index;

	/* Transform as many times as possible.
	*/
	if (inputLen >= partLen)
	{
		//MD5_memcpy((unsigned char *)&context->buffer[index], (unsigned char *)input, partLen);
		memcpy((unsigned char *)&context->buffer[index], (unsigned char *)input, partLen);
		MD5Transform (context->state, context->buffer);

		for (i = partLen; i + 63 < inputLen; i += 64)
		{
			MD5Transform (context->state, &input[i]);
		}

		index = 0;
	}
	else
	{
		i = 0;
	}

	/* Buffer remaining input */
	memcpy((unsigned char *)&context->buffer[index], (unsigned char *)&input[i],\
	       inputLen - i);
}

/* MD5 finalization. Ends an MD5 message-digest operation, writing the
  the message digest and zeroizing the context.
 */
void ez_MD5Final(unsigned char digest[16], __MD5_CTX  *context)
{
	unsigned char bits[8];
	unsigned int index, padLen;

	/* Save number of bits */
	Encode (bits, context->count, 8);

	/* Pad out to 56 mod 64.
	*/
	index = (unsigned int)((context->count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	ez_MD5Update (context, PADDING, padLen);

	/* Append length (before padding) */
	ez_MD5Update (context, bits, 8);

	/* Store state in digest */
	Encode (digest, context->state, 16);

	/* Zeroize sensitive information.
	*/
	memset ((unsigned char *)context, 0, sizeof (*context));
}

/* MD5 basic transformation. Transforms state based on block.
 */
static void MD5Transform(unsigned long state[4], unsigned char block[64])
{
	unsigned long a = state[0], b = state[1], c = state[2], d = state[3], x[16];

	Decode (x, block, 64);

	/* Round 1 */
	FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
	FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
	FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
	FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
	FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
	FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
	FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
	FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
	FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
	FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
	FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
	FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
	FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
	FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
	FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
	FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

	/* Round 2 */
	GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
	GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
	GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
	GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
	GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
	GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
	GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
	GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
	GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
	GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
	GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
	GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
	GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
	GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
	GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
	GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
	HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
	HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
	HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
	HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
	HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
	HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
	HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
	HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
	HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
	HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
	HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
	HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
	HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
	HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
	HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

	/* Round 4 */
	II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
	II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
	II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
	II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
	II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
	II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
	II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
	II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
	II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
	II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
	II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
	II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
	II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
	II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
	II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
	II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	/* Zeroize sensitive information.
	*/
	memset ((unsigned char *)x, 0, sizeof (x));
}

/* Encodes input (unsigned longT4) into output (unsigned char). Assumes len is
  a multiple of 4.
 */
static void Encode(unsigned char *output, unsigned long *input, unsigned int len)
{
	unsigned int i, j;

	for (i = 0, j = 0; j < len; i++, j += 4)
	{
		output[j] = (unsigned char)(input[i] & 0xff);
		output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
		output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
		output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
	}
}

/* Decodes input (unsigned char) into output (unsigned longT4). Assumes len is
  a multiple of 4.
 */
static void Decode(unsigned long *output, unsigned char *input, unsigned int len)
{
	unsigned int i, j;

	for (i = 0, j = 0; j < len; i++, j += 4)
	{
		output[i] = ((unsigned long)input[j]) | (((unsigned long)input[j + 1]) << 8) |
		            (((unsigned long)input[j + 2]) << 16) | (((unsigned long)input[j + 3]) << 24);
	}
}
#if 0
/* Note: Replace "for loop" with standard memcpy if possible.
 */
static void MD5_memcpy(unsigned char * output, unsigned char * input, unsigned int len)
{
	unsigned int i;

	for (i = 0; i < len; i++)
	{
		output[i] = input[i];
	}
}

/* Note: Replace "for loop" with standard memset if possible.
 */
static void MD5_memset(unsigned char * output, int value, unsigned int len)
{
	unsigned int i;

	for (i = 0; i < len; i++)
	{
		((char *)output)[i] = (char)value;
	}
}
#endif
unsigned char *
ez_md5_calc(unsigned char *output, unsigned char *input, unsigned int inlen)
{
	__MD5_CTX context;

	ez_MD5Init(&context);
	ez_MD5Update(&context, input, inlen);
	ez_MD5Final(output, &context);

	return output;
}

unsigned char *
ez_md5_calc_8ascii(unsigned char *strOutput, unsigned char *strInput, unsigned int inlen)
{
	unsigned char outputtemp[16];
	unsigned char *input;
	int i;

	input = strInput;
        int input_len = inlen;
	ez_md5_calc(outputtemp, input, input_len/*strlen((char*)input)*/);

	for (i = 0; i <= 7; i++)
	{
		strOutput[i] = ( (outputtemp[2 * i] + outputtemp[2 * i + 1]) % 62 );

		if (/*(strOutput[i] >= 0) &&*/ (strOutput[i] <= 9))
		{
			strOutput[i] += 48;
		}
		else
		{

			if ((strOutput[i] >= 10) && (strOutput[i] <= 35))
			{
				strOutput[i] += 55;
			}
			else
			{
				strOutput[i] += 61;
			}
		}
	}

	return strOutput;
}

unsigned char *
ez_md5_calc_hexstr(unsigned char *output, unsigned char *input, unsigned int inlen)
{
	int iPrint ;
	char szTemp[4] = {0};
	unsigned char szPassWord[17] = {0};
	ez_md5_calc(szPassWord, input, inlen);

	for (iPrint = 0; iPrint < 16; iPrint++)
	{
		sprintf(szTemp,"%02X",szPassWord[iPrint]);
		strcat((char *)output,szTemp);
	}

	return output;
}

unsigned char *
ez_md5_calc_8ascii_digit(unsigned char *strOutput, unsigned int iOutLen, unsigned char *strInput, unsigned int inlen)
{
	unsigned char EncStr_1[9];
	unsigned char EncStr_2[9];

	unsigned int ii;
	unsigned int kk;

	if (NULL == strOutput
	    || NULL == strInput)
	{
		return NULL;
	}

	ez_md5_calc_8ascii(EncStr_1, strInput, inlen);
	EncStr_1[8] = '\0';

	//
	// chg to num
	//
	for (ii=0; ii<iOutLen; )
	{
		for (kk=0; kk<8; kk++)
		{
			if (isdigit(EncStr_1[kk]))
			{
				*(strOutput+ii) = EncStr_1[kk];

				ii++;

				if (ii>=iOutLen)
				{
					break;
				}
			}
		}

		memcpy(EncStr_2, EncStr_1, sizeof(signed char)*9);
		ez_md5_calc_8ascii(EncStr_1, EncStr_2, inlen);
		EncStr_1[8] = '\0';
	} // for

	return strOutput;
}

unsigned char *
ez_md5_calc_ascii_digit(unsigned char *strOutput, unsigned int iOutLen, unsigned char *strInput, unsigned int inlen)
{
	unsigned char EncStr_1[9];
	unsigned char EncStr_2[9];

	int num_output = iOutLen-1;

	unsigned int ii;
	unsigned int kk;

	if (NULL == strOutput
	    || NULL == strInput
	    || iOutLen < 1
	    || inlen < 1)
	{
		return NULL;
	}

	ez_md5_calc_8ascii(EncStr_1, (unsigned char *)strInput, inlen);
	EncStr_1[8] = '\0';

    //printf("EncStr_1:%s\n", EncStr_1);

	//
	// chg to num
	//
	for (ii=0; ii<num_output; )
	{
		for (kk=0; (kk<8) && (ii<num_output); kk++)
		{
			if (isdigit(EncStr_1[kk]))
			{
				*(strOutput+ii) = EncStr_1[kk];
				ii++;
			}
		}

		memcpy(EncStr_2, EncStr_1, sizeof(signed char)*9);
		ez_md5_calc_8ascii(EncStr_1, EncStr_2, 8);//8 ascii
		EncStr_1[8] = '\0';
	} // for

	return strOutput;
}

#include <time.h>

// super pwd length
#define __SUPER_PWD_LEN 6

signed char *
ez_super_passwd_device(signed char *strDeviceSPwd, int iLen)
{
	//
	// super passwd
	//
	char strSource[32];
	char strSuperPwd[__SUPER_PWD_LEN+1];
	time_t ttNow;
	struct tm *tmNow;

	if (NULL == strDeviceSPwd
	    || iLen < __SUPER_PWD_LEN+1)
	{
		return NULL;
	}

	ttNow = time(NULL);
	tmNow = localtime(&ttNow);

	sprintf(strSource, "%dJ%dO%dY%d",
	        tmNow->tm_hour,
	        tmNow->tm_mday,
	        tmNow->tm_year,
	        tmNow->tm_mon);
	//printf("strSource:%s\n", strSource);

	ez_md5_calc_8ascii_digit((unsigned char *)strSuperPwd,
	                         __SUPER_PWD_LEN,
	                         (unsigned char *)(strSource),
	                         strlen(strSource));
	strSuperPwd[__SUPER_PWD_LEN] = '\0';

	strcpy((char *)strDeviceSPwd, (char *)strSuperPwd);

	return strDeviceSPwd;
}


#define PID_GEN_LEN 7

signed char *
ez_gen_auth_code(char *strAuthCode, int iAuthCodeLen, int iValidSeconds, char *pSN, struct tm *tmNow)
{
	//validSec 10
	// dtime 10
	// psn->s PID_GEN_LEN
	char strSource[20+PID_GEN_LEN+1] = {0};
	if (NULL == strAuthCode
	    || iAuthCodeLen < AUTH_CODE_LEN+1
	    || AUTH_CODE_LEN<11)
	{
		return NULL;
	}

	ez_md5_calc_8ascii_digit((unsigned char *)strSource,
	                         PID_GEN_LEN,
	                         (unsigned char *)(pSN),
	                         strlen(pSN));
	strSource[PID_GEN_LEN] = '\0';

	sprintf(strSource+PID_GEN_LEN, "%04d%02d%010d%02d%02d",
	        tmNow->tm_year,
	        tmNow->tm_mon,
	        iValidSeconds,
	        tmNow->tm_mday,
	        tmNow->tm_hour);


	ez_md5_calc_8ascii_digit((unsigned char *)strAuthCode,
	                         AUTH_CODE_LEN-10,
	                         (unsigned char *)(strSource),
	                         strlen(strSource));
	strAuthCode[AUTH_CODE_LEN-10] = '\0';

	sprintf(strAuthCode+AUTH_CODE_LEN-10, "%d", iValidSeconds);

	return (signed char *)strAuthCode;
}

int ez_get_valid_seconds(char *strAuthCode, int iAuthCodeLen, char *pSN, struct tm *tmNow)
{
	int iValidSeconds=0;
	char __AutoCode[AUTH_CODE_LEN+1];
	if (NULL == strAuthCode
	    || iAuthCodeLen < AUTH_CODE_LEN+1-10
	    || AUTH_CODE_LEN<11)
	{
		printf("param error.\n");
		return -1;
	}

	iValidSeconds = atoi(strAuthCode+AUTH_CODE_LEN-10);


	ez_gen_auth_code(__AutoCode, AUTH_CODE_LEN+1, iValidSeconds, pSN, tmNow);
	if (strcmp(__AutoCode, strAuthCode) == 0)
	{
		return iValidSeconds;
	}
	else
	{
		printf("sn:%s __AutoCode%s, strAuthCode:%s\n"
		, pSN
		, __AutoCode
		, strAuthCode);

		return -1;
	}
}

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#else//if _LINUX
#include <unistd.h>
#endif

#include <assert.h>

int ez_file_md5(char *output, char *p_filename)
{

    int ret=0;
    char buf[4096];
    int from, rcc;

	__MD5_CTX context;
	char szTemp[4] = {0};
	unsigned char outputtemp[16]={0};
	int iPrint=0;

	assert(output);
	assert(p_filename);

    from = open(p_filename, O_RDONLY);

    ez_MD5Init(&context);

    do
    {
        rcc = read(from, buf, sizeof(buf));

        if (rcc>0)
        {
            ez_MD5Update(&context, (unsigned char *)buf, rcc);
        }
        else if (rcc == 0)
        {
            // read finished cal it
            output[0]='\0'; // set string len to 0
            ez_MD5Final(outputtemp, &context);

            //printf("MD5:");
            for (iPrint = 0; iPrint < 16; iPrint++)
            {
                sprintf(szTemp,"%02X",outputtemp[iPrint]);
                strcat((char *)output, szTemp);
            }
            ret = strlen(output);

            break;
        }
        else// if (rcc < 0)
        {
            ret = -100;
            printf("read error, errno=%d\n", errno);
            break;
        }
    }
    while(rcc>0);

    close(from);

    return ret;
}
