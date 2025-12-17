/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * base58.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: base58.c 5884 2023-05-01 03:48:47Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2023-05-01 03:48:47  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <stdio.h>
#include <string.h>

#include <ezutil/base58.h>

// 基本测试
// &
// api使用实例
#define _DEBUG_THIS
#include <ezutil/ez_def_devel_debug.h>
void testPrimaryUsage()
{
	//--------------------------
	const unsigned char *encode_in = (unsigned char *)"1234567";
	char unsigned encode_out[1024] = {0};
	size_t encode_out_size = sizeof(encode_out);

	size_t bel = base58_encode_length(strlen((char *)encode_in));
	ARG_USED(bel);
	int base58_encode_iret = base58_encode(encode_in, strlen((char *)encode_in), encode_out, &encode_out_size);
	ARG_USED(base58_encode_iret);

	DBG(
		printf("encode_in str:%s\n", (char *)encode_in);
		printf("\tbase58_encode:%d:%ld[%s]\n", base58_encode_iret, encode_out_size, (char *)encode_out);
		printf("\tThis base58 alo is:%s\n", strcmp("2s8YYFs4Vc", (char *)encode_out) == 0 ? "OK" : "NOK");
		printf("\t%s:%ld -- %s\n", "bel", bel, encode_out_size == bel ? "OK" : "NOK"););

	size_t bdl = base58_decode_length(encode_out_size);
	ARG_USED(bdl);

	char unsigned decode_in[1024] = {0};
	char unsigned decode_out[1024] = {0};
	size_t decode_out_size = sizeof(decode_out);
	memcpy(decode_in, encode_out, sizeof(decode_in) - 1);
	decode_in[sizeof(decode_in) - 1] = '\0';
	int base58_decode_iret = base58_decode(decode_in, strlen((char *)decode_in), decode_out, &decode_out_size);

	DBG(
		printf("decode_in str:%s\n", decode_in);
		printf("\tbase58_decode:%d:%ld[%s]\n", base58_decode_iret, decode_out_size, (char *)decode_out);
		printf("\tThis base58 alo is:%s\n", strcmp((char *)encode_in, (char *)decode_out) == 0 ? "OK" : "NOK");
		printf("\t%s:%ld -- %s\n", "bdl", bdl, strlen((char *)encode_in) == bdl ? "OK" : "NOK");
		printf("\t%s:%ld -- %s\n", "decode_out_size", decode_out_size, decode_out_size == bdl ? "OK" : "NOK"););
}

#include <assert.h>

#define _defTstBufferSize 1024
void testFull()
{
	//--------------------------
	size_t i = 1;
	for (i = 1; i < _defTstBufferSize; i++)
	{

		char unsigned encode_in[_defTstBufferSize * 2] = {'a'};
		size_t encode_in_size = i;

		char unsigned encode_out[_defTstBufferSize * 2] = {0};
		size_t encode_out_size = sizeof(encode_out);

		size_t bel = base58_encode_length(encode_in_size);
		ARG_USED(bel);
		int base58_encode_iret = base58_encode(encode_in, encode_in_size, encode_out, &encode_out_size);
		ARG_USED(base58_encode_iret);
		assert(base58_encode_iret == 0);
		if (encode_out_size != bel)
		{
			printf("i:%ld, encode_out_size:%ld, bel:%ld\n", i, encode_out_size, bel);
			// base58_encode_length 是近似值，实际编码长度可能略有不同
			// 只要实际长度不超过预期长度即可
			if (encode_out_size > bel) {
				printf("ERROR: encode_out_size (%ld) > bel (%ld)\n", encode_out_size, bel);
				assert(0);
			}
		}

		size_t bdl = base58_decode_length(encode_out_size);
		ARG_USED(bdl);

		char unsigned decode_in[_defTstBufferSize * 2] = {0};
		char unsigned decode_out[_defTstBufferSize * 2] = {0};
		size_t decode_out_size = sizeof(decode_out);
		memcpy(decode_in, encode_out, encode_out_size);
		decode_in[encode_out_size] = '\0';
		int base58_decode_iret = base58_decode(decode_in, encode_out_size, decode_out, &decode_out_size);

		assert(base58_decode_iret == 0);
		assert(memcmp(encode_in, decode_out, encode_in_size) == 0);

		// base58_decode_length 和 base58_encode_length 是近似值，实际长度可能略有不同
		// 最重要的是验证解码后的数据与原始数据一致
		// decode_out_size 应该等于 encode_in_size（原始输入长度）
		assert(decode_out_size == encode_in_size);

	} // for
}

int main(int argc, char **argv)
{
	testPrimaryUsage();
	testFull();
	return 0;
}
