#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ezutil/std_md5.h"
#include "ezutil/ez_md5.h"

char * newDigestResponse( \
    const char *username
    , const char *realm
    , const char *password
    , const int passwordIsMd5
    , const char *nonce
    , const char *public_method
    , const char *uri);


int main(int argc, char *argv[])
{
    // 使用基本接口
    {
        // char *pPwd="wx@123"; //044B383F001BB83EE435E378C350ADC6
        char *pPwd = "xqyanshi@1"; // zhengjiang
        std_MD5_CTX ctx;
        unsigned char outputtemp[32] = {0};
        int iPrint;

        std_MD5_Init(&ctx);

        std_MD5_Update(&ctx, pPwd, strlen(pPwd));

        std_MD5_Final(outputtemp, &ctx);

        // display result
        for (iPrint = 0; iPrint < 16; iPrint++)
        {
            printf("%02X ", outputtemp[iPrint]);
        }
        printf("\n");

        char stdMd5Buf[EZ_STD_MD5_RESULT_LEN] = {0};
        ez_cal_std_md5(stdMd5Buf, EZ_STD_MD5_RESULT_LEN, pPwd, strlen(pPwd));
        assert(memcmp(stdMd5Buf, outputtemp, EZ_STD_MD5_RESULT_LEN) == 0);
    }
    // 使用封装接口
    {
        char *pPwd = "wx@123"; // 044B383F001BB83EE435E378C350ADC6
        char out[EZ_STD_MD5_STRING_LEN_UP_32B + 1] = {'\0'};
        int iret = ez_cal_std_md5_32b_up_string(out, sizeof(out), pPwd, strlen(pPwd));
        if (iret < 0)
        {
            printf("ez_cal_std_md5_32b_up_string failed:%d\n", iret);
        }
        else
        {
            printf("ez_cal_std_md5_32b_up_string:%s\n", out);
        }
    }
    // 使用封装接口
    {
        // char *pPwd="wx@123"; //044B383F001BB83EE435E378C350ADC6
        char *pPwd = "xqyanshi@1"; // zhengjiang
        //
        char out[EZ_STD_MD5_STRING_LEN_UP_32B + 1] = {'\0'};
        printf("ez_cal_std_md5_32b_low_string pPwd:%s\n", pPwd);
        int iret = ez_cal_std_md5_32b_low_string(out, sizeof(out), pPwd, strlen(pPwd));
        if (iret < 0)
        {
            printf("ez_cal_std_md5_32b_low_string failed:%d\n", iret);
        }
        else
        {
            printf("ez_cal_std_md5_32b_low_string:%s\n", out);
        }
    }


    // digest
    {
        char *username = "admin";
        char *realm = "4419b6ab2aaa";
        char *password = "GWXUAA";
        int passwordIsMd5 = 0;
        char *nonce = "401797400437bf1800845d54bbf3ca88";
        char *public_method = "DESCRIBE";
        char *uri = "rtsp://admin:GWXUAA@192.168.9.61:554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1";

        char *pret = newDigestResponse(username, realm, password, passwordIsMd5, nonce, public_method, uri);

        if (pret)
        {
            printf("pret:%s\n", pret);
            // assert(strcmp("2f6e523b9aafede9242497c240c8eaa3", stdMd5Res)==0);
            free(pret);
            pret = NULL;
        }
    }

    return 0;
}

// digest
// 参考代码：vi ./liveMedia/DigestAuthentication.cpp
//参考消息：./testProgs/openRTSP 'rtsp://admin:GWXUAA@192.168.9.61:554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1'
// Sending request: DESCRIBE rtsp://admin:GWXUAA@192.168.9.61:554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1 RTSP/1.0
// CSeq: 4
// Authorization: Digest username="admin", realm="4419b6ab2aaa", nonce="401797400437bf1800845d54bbf3ca88", uri="rtsp://admin:GWXUAA@192.168.9.61:554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1", response="2f6e523b9aafede9242497c240c8eaa3"
// User-Agent: ./testProgs/openRTSP (LIVE555 Streaming Media v2022.02.07)
// Accept: application/sdp

char * newDigestResponse( \
    const char *username
    , const char *realm
    , const char *password
    , const int passwordIsMd5
    , const char *nonce
    , const char *public_method
    , const char *uri)
{
    // char *username = "admin";
    // char *realm = "4419b6ab2aaa";
    // char *password = "GWXUAA";
    // int passwordIsMd5 = 0;
    // char *nonce = "401797400437bf1800845d54bbf3ca88";
    // char *public_method = "DESCRIBE";
    // char *uri = "rtsp://admin:GWXUAA@192.168.9.61:554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1";

    char stdMd5URM[EZ_STD_MD5_STRING_LEN_UP_32B + 1] = {0};
    char stdMd5Url[EZ_STD_MD5_STRING_LEN_UP_32B + 1] = {0};
    char stdMd5Res[EZ_STD_MD5_STRING_LEN_UP_32B + 1] = {0};
#define tmpBufLen 128
    char buf[tmpBufLen + 1] = {0};

    if (1 == passwordIsMd5)
    {
        strncpy(stdMd5URM, password, EZ_STD_MD5_STRING_LEN_UP_32B);
        // ha1Buf[32] = '\0'; // just in case
    }
    else
    {
        snprintf(buf, tmpBufLen, "%s:%s:%s", username, realm, password);
        ez_cal_std_md5_32b_low_string(stdMd5URM, sizeof(stdMd5URM), buf, strlen(buf));
    }
    printf("stdMd5URM:%s\n", stdMd5URM);

    snprintf(buf, tmpBufLen, "%s:%s", public_method, uri);
    ez_cal_std_md5_32b_low_string(stdMd5Url, sizeof(stdMd5Url), buf, strlen(buf));

    snprintf(buf, tmpBufLen, "%s:%s:%s", stdMd5URM, nonce, stdMd5Url);
    ez_cal_std_md5_32b_low_string(stdMd5Res, sizeof(stdMd5Res), buf, strlen(buf));
    printf("stdMd5Res:%s\n", stdMd5Res);
    assert(strcmp("2f6e523b9aafede9242497c240c8eaa3", stdMd5Res) == 0);

    return NULL;
}
