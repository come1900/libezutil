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
#include <stdlib.h>
#include <string.h>

#include <ezutil/base58.h>
#include <ezutil/libbase58.h>

// 基本测试
// &
// api使用实例
#define _DEBUG_THIS
#include <ezutil/ez_def_devel_debug.h>
void testPrimaryUsage(char *args)
{
	//--------------------------
	const char *encode_in = args; //"123456789";
	// char unsigned encode_out[1024] = {0};
	// size_t encode_out_size = sizeof(encode_out);

	size_t bel = base58_encode_length(strlen((char *)encode_in));
	ARG_USED(bel);

	size_t encode_out_size = bel+1;
	char *encode_out = (char *) malloc(encode_out_size);
    memset(encode_out, 0, encode_out_size);

    
	// int base58_encode_iret = base58_encode(encode_in, strlen((char *)encode_in), encode_out, &encode_out_size);
    bool base58_encode_iret = b58enc(encode_out, &encode_out_size, (char *)encode_in, strlen((char *)encode_in));
	ARG_USED(base58_encode_iret);

	DBG(
		printf("encode_in str:%s\n", (char *)encode_in);
		printf("\t b58enc:%d:%ld[%s]\n", base58_encode_iret, encode_out_size, (char *)encode_out);
        if (strcmp(encode_in, "1234567")==0)
		    printf("\t This base58 alo is:%s\n", strcmp("2s8YYFs4Vc", (char *)encode_out) == 0 ? "OK" : "NOK");
		printf("\t bel:%ld, encode_out_size:%ld\n", bel, encode_out_size);
		printf("\t %s:%ld==%ld==%ld? -- %s\n", "out_size", bel, encode_out_size, strlen(encode_out), encode_out_size-1/*返回的包括尾符*/ <= bel ? "OK" : "NOK");
    );

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
	size_t bdl = base58_decode_length(strlen(encode_out)/*encode_out_size-1*/);
	ARG_USED(bdl);

	char  decode_in[1024] = {0};
	char  decode_out[1024] = {0};
	size_t decode_out_size = sizeof(decode_out);
	strncpy((char *)decode_in, (char *)encode_out, sizeof(decode_in) - 1);
    size_t rt = strlen(decode_in);
	// int base58_decode_iret = base58_decode(decode_in, strlen((char *)decode_in), decode_out, &decode_out_size);
    bool base58_decode_iret = b58tobin(decode_out, &decode_out_size, decode_in, rt);

    // 结果处理一下, 放在缓存后面了
    char cbin[1024]={0};
    memcpy(cbin, &decode_out[1024 - decode_out_size], decode_out_size);

	DBG(
		printf("decode_in str:%s\n", decode_in);
		printf("\t 58tobin:%d:%ld[%s]\n", base58_decode_iret, decode_out_size, (char *)cbin);
		printf("\t This base58 alo is:%s\n", strcmp((char *)encode_in, (char *)cbin) == 0 ? "OK" : "NOK");
		printf("\t %s:%ld -- %s\n", "bdl", bdl, strlen((char *)encode_in) == bdl ? "OK" : "NOK");
		printf("\t %s:%ld -- %s\n", "decode_out_size", decode_out_size, decode_out_size == bdl ? "OK" : "NOK");
    );

    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    char *bin = (char *) malloc (bdl+1); // +1只是为了显示
	size_t bin_size = bdl;
    bin[bin_size]='\0';

	// char  decode_in[1024] = {0};
	// char  decode_out[1024] = {0};
	strncpy((char *)decode_in, (char *)encode_out, sizeof(decode_in) - 1);
    size_t b58len = strlen(decode_in);
    bool b58tobin_bret = b58tobin(bin, &bin_size, decode_in, b58len);

    // 结果处理一下
    // char cbin[1024]={0};
    // memcpy(cbin, &decode_out[1024 - decode_out_size], decode_out_size);

	DBG(
		printf("decode_in str:%s\n", decode_in);
		printf("\t 58tobin:%d:%ld[%s]\n", b58tobin_bret, bin_size, (char *)bin);
		printf("\t This base58 alo is:%s\n", strcmp((char *)encode_in, (char *)bin) == 0 ? "OK" : "NOK");
		printf("\t %s:%ld -- %s\n", "bdl", bdl, strlen((char *)encode_in) == bdl ? "OK" : "NOK");
		printf("\t %s:%ld, bdl:%ld -- %s\n", "decode_out_size", bin_size, bdl, bin_size == bdl ? "OK" : "NOK");
    );

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
		if (encode_out_size != bel)
		{
			printf("i:%ld, encode_out_size:%ld, bel:%ld\n", i, encode_out_size, bel);
		}
		assert(base58_encode_iret == 0);
		assert(encode_out_size == bel);

		size_t bdl = base58_decode_length(encode_out_size);
		ARG_USED(bdl);

		char unsigned decode_in[_defTstBufferSize * 2] = {0};
		char unsigned decode_out[_defTstBufferSize * 2] = {0};
		size_t decode_out_size = sizeof(decode_out);
		memcpy(decode_in, encode_out, sizeof(decode_in) - 1);
		decode_in[sizeof(decode_in) - 1] = '\0';
		int base58_decode_iret = base58_decode(decode_in, encode_in_size, decode_out, &decode_out_size);

		assert(base58_decode_iret == 0);
		assert(memcmp(encode_in, decode_out, encode_in_size));

		if (decode_out_size != bdl)
		{
			printf("i:%ld, decode_out_size:%ld, bdl:%ld\n", i, decode_out_size, bdl);
		}
		assert(decode_out_size == bdl);

	} // for
}

int main(int argc, char **argv)
{
    char *tstString = argc>1?argv[1]:"1234567";
	testPrimaryUsage(tstString);
	// testFull();
	return 0;
}
