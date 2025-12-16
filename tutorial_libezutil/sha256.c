/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * sha256.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: sha256.c 5884 2021-10-31 10:43:15Z WuJunjie $
 *
 *  Explain:
 *     -
 *      UT&Exa for sha256
 *      to https://sotool.net/sha256 
 *      sha256(abc) is: 
 *      ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad
 *     -
 *
 *  Run:
 *     rm -f sha256.o ; make && ./sha256.o
 * 
 *  Update:
 *     2021-10-31 10:43:15  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <stdio.h>
#include <memory.h>
#include <string.h>

#include "sha256.h"
#include <string.h>
#include <stdint.h>
#include <assert.h>


int sha256_test()
{
	const char *text1 = "abc";
	const char *text2 = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	const char *text3 = "aaaaaaaaaa";
	unsigned char hash1[SHA256_BLOCK_SIZE] = {0xba,0x78,0x16,0xbf,0x8f,0x01,0xcf,0xea,0x41,0x41,0x40,0xde,0x5d,0xae,0x22,0x23,
	                                 0xb0,0x03,0x61,0xa3,0x96,0x17,0x7a,0x9c,0xb4,0x10,0xff,0x61,0xf2,0x00,0x15,0xad};
	unsigned char hash2[SHA256_BLOCK_SIZE] = {0x24,0x8d,0x6a,0x61,0xd2,0x06,0x38,0xb8,0xe5,0xc0,0x26,0x93,0x0c,0x3e,0x60,0x39,
	                                 0xa3,0x3c,0xe4,0x59,0x64,0xff,0x21,0x67,0xf6,0xec,0xed,0xd4,0x19,0xdb,0x06,0xc1};
	unsigned char hash3[SHA256_BLOCK_SIZE] = {0xcd,0xc7,0x6e,0x5c,0x99,0x14,0xfb,0x92,0x81,0xa1,0xc7,0xe2,0x84,0xd7,0x3e,0x67,
	                                 0xf1,0x80,0x9a,0x48,0xa4,0x97,0x20,0x0e,0x04,0x6d,0x39,0xcc,0xc7,0x11,0x2c,0xd0};
	unsigned char buf[SHA256_BLOCK_SIZE];
	SHA256_CTX ctx;
	int idx;

	int pass = 1;

	sha256_init(&ctx);
	sha256_update(&ctx, (unsigned char *)text1, strlen(text1));
	sha256_final(&ctx, buf);
	pass = pass && !memcmp(hash1, buf, SHA256_BLOCK_SIZE);

	sha256_init(&ctx);
	sha256_update(&ctx, (unsigned char *)text2, strlen(text2));
	sha256_final(&ctx, buf);
	pass = pass && !memcmp(hash2, buf, SHA256_BLOCK_SIZE);

	sha256_init(&ctx);
	for (idx = 0; idx < 100000; ++idx)
	   sha256_update(&ctx, (unsigned char *)text3, strlen(text3));
	sha256_final(&ctx, buf);
	pass = pass && !memcmp(hash3, buf, SHA256_BLOCK_SIZE);

	return(pass);
}

int hmac_sha256_test()
{
	uint8_t out[32];
	int pass = 1;

	// Test case 1: RFC 4231 test vector
	// key = 0x0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b (20 bytes)
	// data = "Hi There" (8 bytes)
	// expected = 0xb0344c61d8db38535ca8afceaf0bf12b881dc200c9833da726e9376c2e32cff7
	const uint8_t key1[20] = {0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,
	                          0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b};
	const char *msg1 = "Hi There";
	const uint8_t expected1[32] = {0xb0,0x34,0x4c,0x61,0xd8,0xdb,0x38,0x53,0x5c,0xa8,
	                               0xaf,0xce,0xaf,0x0b,0xf1,0x2b,0x88,0x1d,0xc2,0x00,
	                               0xc9,0x83,0x3d,0xa7,0x26,0xe9,0x37,0x6c,0x2e,0x32,
	                               0xcf,0xf7};

	int ret = ez_hmac_sha256_manual(key1, 20, (const uint8_t *)msg1, strlen(msg1), out);
	pass = pass && (ret == 0) && !memcmp(expected1, out, 32);

	// Test case 2: RFC 4231 test vector
	// key = "Jefe" (4 bytes)
	// data = "what do ya want for nothing?" (28 bytes)
	// expected = 0x5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843
	const char *key2 = "Jefe";
	const char *msg2 = "what do ya want for nothing?";
	const uint8_t expected2[32] = {0x5b,0xdc,0xc1,0x46,0xbf,0x60,0x75,0x4e,0x6a,0x04,
	                               0x24,0x26,0x08,0x95,0x75,0xc7,0x5a,0x00,0x3f,0x08,
	                               0x9d,0x27,0x39,0x83,0x9d,0xec,0x58,0xb9,0x64,0xec,
	                               0x38,0x43};

	ret = ez_hmac_sha256_manual((const uint8_t *)key2, strlen(key2), 
	                            (const uint8_t *)msg2, strlen(msg2), out);
	pass = pass && (ret == 0) && !memcmp(expected2, out, 32);

	// Test case 3: RFC 4231 test vector - key longer than 64 bytes
	// key = 0xaa repeated 131 times
	// data = "Test Using Larger Than Block-Size Key - Hash Key First" (54 bytes)
	// expected = 0x60e431591ee0b67f0d8a26aacbf5b77f8e0bc6213728c5140546040f0ee37f54
	uint8_t key3[131];
	memset(key3, 0xaa, 131);
	const char *msg3 = "Test Using Larger Than Block-Size Key - Hash Key First";
	const uint8_t expected3[32] = {0x60,0xe4,0x31,0x59,0x1e,0xe0,0xb6,0x7f,0x0d,0x8a,
	                               0x26,0xaa,0xcb,0xf5,0xb7,0x7f,0x8e,0x0b,0xc6,0x21,
	                               0x37,0x28,0xc5,0x14,0x05,0x46,0x04,0x0f,0x0e,0xe3,
	                               0x7f,0x54};

	ret = ez_hmac_sha256_manual(key3, 131, (const uint8_t *)msg3, strlen(msg3), out);
	pass = pass && (ret == 0) && !memcmp(expected3, out, 32);

	// Test case 4: Parameter validation tests
	// Test NULL output buffer
	ret = ez_hmac_sha256_manual(key1, 20, (const uint8_t *)msg1, strlen(msg1), NULL);
	pass = pass && (ret == -101);

	// Test NULL key with non-zero length
	ret = ez_hmac_sha256_manual(NULL, 20, (const uint8_t *)msg1, strlen(msg1), out);
	pass = pass && (ret == -102);

	// Test NULL message with non-zero length
	ret = ez_hmac_sha256_manual(key1, 20, NULL, 10, out);
	pass = pass && (ret == -103);

	// Test empty key (should work)
	ret = ez_hmac_sha256_manual(NULL, 0, (const uint8_t *)msg1, strlen(msg1), out);
	pass = pass && (ret == 0);

	// Test empty message (should work)
	ret = ez_hmac_sha256_manual(key1, 20, NULL, 0, out);
	pass = pass && (ret == 0);

	return pass;
}

int main(int argc, char *argv[])
{
	printf("SHA-256 tests: %s\n", sha256_test() ? "SUCCEEDED" : "FAILED");
	printf("HMAC-SHA256 tests: %s\n", hmac_sha256_test() ? "SUCCEEDED" : "FAILED");

	return(0);
}
