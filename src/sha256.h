/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * sha256.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: sha256.h 5884 2021-10-31 10:30:40Z WuJunjie $
 *
 *  Explain:
 *     -
 *      From:https://github.com/B-Con/crypto-algorithms.git
 *      modified for ezlibs.com
 *     -
 *
 *  Update:
 *     2021-10-31 10:30:40  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef SHA256_H
#define SHA256_H

/*************************** HEADER FILES ***************************/
#include <stddef.h>

/****************************** MACROS ******************************/
#define SHA256_BLOCK_SIZE 32            // SHA256 outputs a 32 unsigned char digest

typedef struct {
	unsigned char data[64];
	unsigned int datalen;
	unsigned long long bitlen;
	unsigned int state[8];
} SHA256_CTX;

/*********************** FUNCTION DECLARATIONS **********************/
#ifdef __cplusplus
extern "C"
{
#endif
void sha256_init(SHA256_CTX *ctx);
// must be unsigned
void sha256_update(SHA256_CTX *ctx, const unsigned char data[], size_t len);
void sha256_final(SHA256_CTX *ctx, unsigned char hash[]);
#ifdef __cplusplus
}
#endif

#endif   // SHA256_H
