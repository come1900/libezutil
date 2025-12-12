// AES128
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "str_opr.h"
#include "aes.h"

int main()
{
    int exit = 0;
    const char *pKey = "1234567890123456";
    const char *pIv = pKey;
    //char *pMsg = "hello";
    const char *pMsg = "hello";

    int iMsgLen = strlen(pMsg)+1;
    int iKeyLen = strlen(pKey)+1;
    int MsgAesBufLen;
    char *bufOut = NULL;

    char *bufOutOri = NULL;

    if (iKeyLen<AES_KEYLEN)
    {
        printf("KeyLen should not less:%d\n", AES_KEYLEN);

        return -1;
    }

    //aesBufLen = AES_BLOCKLEN*((int)(iMsgLen/AES_BLOCKLEN))
    //   + AES_BLOCKLEN*((iMsgLen%AES_BLOCKLEN)>0?1:0);

    MsgAesBufLen = ((int)((iMsgLen+AES_BLOCKLEN-1)/AES_BLOCKLEN)) * AES_BLOCKLEN;
    printf("iMsgLen:%d\n", iMsgLen);
    printf("MsgAesBufLen:%d\n", MsgAesBufLen);

    {
        bufOut = (char *)malloc(MsgAesBufLen);
        memset(bufOut, 0, MsgAesBufLen);
        memcpy(bufOut, pMsg, iMsgLen);

        //
        struct AES_ctx ctx;
        AES_init_ctx_iv(&ctx, (uint8_t *)pKey, (uint8_t *)pIv);
        AES_CBC_encrypt_buffer(&ctx, (uint8_t *)bufOut, MsgAesBufLen);
    }
    {
        bufOutOri = (char *)malloc(MsgAesBufLen);
        memset(bufOutOri, 0, MsgAesBufLen);
        memcpy(bufOutOri, bufOut, MsgAesBufLen);

        struct AES_ctx ctxDec;
        AES_init_ctx_iv(&ctxDec, (uint8_t *)pKey, (uint8_t *)pIv);
        AES_CBC_decrypt_buffer(&ctxDec, (uint8_t *)bufOutOri, MsgAesBufLen);
    }

    printf("%s:%ld\n", "strlen(bufOutOri)", strlen(bufOutOri));
    printf("%s:%d\n", "sizeof(bufOutOri)", MsgAesBufLen);
    printf("%s:%s\n", "bufOutOri", bufOutOri);

    return exit;
}

