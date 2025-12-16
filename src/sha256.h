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
#include <stdint.h>

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

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ez_hmac_sha256_manual - HMAC-SHA256 implementation
 *
 * @key:      pointer to the key buffer
 * @key_len:  length of the key in bytes
 * @msg:      pointer to the message buffer
 * @msg_len:  length of the message in bytes
 * @out:      output buffer for HMAC-SHA256 result (must be at least 32 bytes)
 *
 * Return:
 *     0     - success
 *     -101  - invalid parameter: output buffer is NULL
 *     -102  - invalid parameter: key is NULL when key_len > 0
 *     -103  - invalid parameter: message is NULL when msg_len > 0
 *
 * Explain:
 *     Compute HMAC-SHA256 according to RFC 2104.
 *     If key_len > 64, the key is first hashed using SHA256.
 *     Empty key (key_len == 0) and empty message (msg_len == 0) are allowed.
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
int ez_hmac_sha256_manual(const uint8_t *key, size_t key_len,
	const uint8_t *msg, size_t msg_len,
	uint8_t out[32]);

#ifdef __cplusplus
}
#endif

#endif   // SHA256_H
