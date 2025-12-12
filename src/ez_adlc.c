/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ez_adlc.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ez_adlc.c 5884 2022-07-13 08:26:13Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2022-07-13 08:26:13  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "ez_adlc.h"

#define ARG_USED(x) (void)&x;

ADLC_HANDLER_T *adlc_create(const unsigned int maxPacketLen, const unsigned char packetFlag, CALLBACK_ON_ADLC * pOnAdlc)
{
    if (maxPacketLen < 2)
        return NULL;

    ADLC_HANDLER_T *pHandler = (ADLC_HANDLER_T *)calloc(sizeof(ADLC_HANDLER_T), 1);
    // assert(pHandler);

    pHandler->rxBuf = (unsigned char *)calloc(maxPacketLen, 1);
    pHandler->rxBufLen = 0;
    pHandler->execBuf = (unsigned char *)calloc(maxPacketLen, 1);
    pHandler->execBufLen = 0;

    // private
    pHandler->pCbAdlc = pOnAdlc;
    pHandler->iMaxPacketLen = maxPacketLen;
    pHandler->headFlag = packetFlag;

    return pHandler;
}

int adlc_destroy(ADLC_HANDLER_T *pHandler)
{
    if (NULL == pHandler)
        return -1;

    if (pHandler->rxBuf != NULL)
    {
        free(pHandler->rxBuf);
        pHandler->rxBuf = NULL;
    }

    if (pHandler->execBuf != NULL)
    {
        free(pHandler->execBuf);
        pHandler->execBuf = NULL;
    }

    free(pHandler);

    return 0;
}

int adlc_ondata(ADLC_HANDLER_T *pHandler, unsigned char *pData, unsigned int dataLen)
{
    if (NULL == pHandler || NULL == pData || dataLen < 1)
    {
        return -1;
    }

    if (pHandler->rxBufLen > 0)
    {
        memcpy(pHandler->execBuf, pHandler->rxBuf, pHandler->rxBufLen);
        pHandler->execBufLen = pHandler->rxBufLen;
        pHandler->rxBufLen = 0;
    }
    
    unsigned int i=0;

    for (i = 0; i < dataLen; i++)
    {
        if (pHandler->headFlag != pData[i])
        {
            // 跳过垃圾数据
            if (0==pHandler->execBufLen || pHandler->execBuf[0] != pHandler->headFlag)
                continue;

            pHandler->execBuf[pHandler->execBufLen] = pData[i];
            pHandler->execBufLen++;

            // 强制释放
            if (pHandler->pCbAdlc &&
                pHandler->execBufLen >= pHandler->iMaxPacketLen)
            {
                pHandler->pCbAdlc(pHandler->execBuf, pHandler->execBufLen, NULL);
                pHandler->execBufLen = 0;
            } //强制释放
        }
        else
        {
            if (pHandler->pCbAdlc && pHandler->execBufLen>0)
                pHandler->pCbAdlc(pHandler->execBuf, pHandler->execBufLen, NULL);

            pHandler->execBufLen = 0;
            pHandler->execBuf[pHandler->execBufLen] = pData[i];
            pHandler->execBufLen++;
        }
    }

    if (pHandler->execBufLen <= 0)
        return 1;

    if (pHandler->execBuf[0] != pHandler->headFlag)
    {
        pHandler->execBufLen = 0;
        return 1;
    }

    // 检查最后的数据
    if (pHandler->pCbAdlc)
    {
        int iret = pHandler->pCbAdlc(pHandler->execBuf, pHandler->execBufLen, NULL);

        if (iret >= 0)
        {
            pHandler->execBufLen = 0;
        }
        else //不完整包
        {
            //存到缓存中
            memcpy(pHandler->rxBuf, pHandler->execBuf, pHandler->execBufLen);
            pHandler->rxBufLen = pHandler->execBufLen;
        }
    }

    return 0;
}
