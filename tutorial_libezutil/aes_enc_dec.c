// AES128
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "str_opr.h"
#include "aes.h"

int main()
{
    int exit = 0;
    //const char *pKey = "1234567890123456";
    const char *pKey = "1";
    const char *pMsg = "hello";

    int iMsgLen = strlen(pMsg)+1;
    int iKeyLen = strlen(pKey);
    int MsgAesBufLen;
    char *bufOut = NULL;

    char *bufOutOri = NULL;

    if (iKeyLen<AES_KEYLEN)
    {
        printf("Warning:: KeyLen should not less:%d\n", AES_KEYLEN);

        //return -1;
    }

    MsgAesBufLen = ez_aes_cbc_encrypt_out_len(iMsgLen);//((int)((iMsgLen+AES_BLOCKLEN-1)/AES_BLOCKLEN)) * AES_BLOCKLEN;
    printf("iMsgLen:%d\n", iMsgLen);
    printf("MsgAesBufLen:%d\n", MsgAesBufLen);

    //
    bufOut = (char *)malloc(MsgAesBufLen);
    assert(bufOut);
    int encDataLen = ez_aes_cbc_encrypt(bufOut, MsgAesBufLen, pMsg, iMsgLen, pKey, strlen(pKey));

    //bufOutOri 大小一般等于in
    bufOutOri = (char *)malloc(MsgAesBufLen);
    assert(bufOutOri);
    ez_aes_cbc_decrypt(bufOutOri, MsgAesBufLen, bufOut, encDataLen, pKey, strlen(pKey));

    printf("%s:%ld\n", "strlen(bufOutOri)", strlen(bufOutOri));
    printf("%s:%d\n", "sizeof(bufOutOri)", MsgAesBufLen);
    printf("%s:%s\n", "bufOutOri", bufOutOri);

    free (bufOut);
    free (bufOutOri);

    return exit;
}

