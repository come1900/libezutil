/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ez_hdlc.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ez_hdlc.c 5884 2015-03-04 05:03:18Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2015-03-04 05:03:18  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "crc.h"
#include "ez_hdlc.h"

#define __EZ_HDLC_RX_BUFF_LEN EZ_HDLC_BUFF_LEN*2

HDLC_HANDLER_T *hdlc_new()
{
    HDLC_HANDLER_T *p_handler = (HDLC_HANDLER_T *)malloc(sizeof(HDLC_HANDLER_T));
    assert(p_handler);

    p_handler->iInterface = 0; // 接口编号
    p_handler->pRxBuffer = (unsigned char *)malloc(__EZ_HDLC_RX_BUFF_LEN); // 接收缓存
    assert(p_handler->pRxBuffer);
    p_handler->iRxBufferLen = 0; // 接收缓存数据长度
#if defined(EZ_HDLC_TX_BUFF_LEN) && EZ_HDLC_TX_BUFF_LEN>0

    p_handler->pTxBuffer = (unsigned char *)malloc(EZ_HDLC_TX_BUFF_LEN); // 发送缓存
    assert(p_handler->pTxBuffer);
    p_handler->iTxBufferLen = 0; // 发送缓存数据长度
#endif //EZ_HDLC_TX_BUFF_LEN

    p_handler->pHdlcCallback  = NULL;

    return p_handler;
}

int hdlc_delete(HDLC_HANDLER_T *p_handler)
{
    if (p_handler)
    {
        if (p_handler->pRxBuffer)
        {
            free(p_handler->pRxBuffer);
            p_handler->pRxBuffer = NULL;
        }
#if defined(EZ_HDLC_TX_BUFF_LEN) && EZ_HDLC_TX_BUFF_LEN>0
        if (p_handler->pTxBuffer)
        {
            free(p_handler->pTxBuffer);
            p_handler->pTxBuffer = NULL;
        }
#endif //EZ_HDLC_TX_BUFF_LEN
        free(p_handler);
        p_handler = NULL;

        return 0;
    }

    return -1;
}

//return hdlc num
int hdlc_ondata(HDLC_HANDLER_T *p_handler, unsigned char *p_data, unsigned int i_len, void *obj)
{
    unsigned int ii=0;
    unsigned int jj=0;
    int hdlcNum = 0;

    if (p_handler==NULL
        || p_data==NULL
        || i_len==0)
    {
        return -1;
    }

    // overload
    if (p_handler->iRxBufferLen+i_len>__EZ_HDLC_RX_BUFF_LEN)
    {
        p_handler->iRxBufferLen = 0;
        return -2;
    }

    memcpy( (p_handler->pRxBuffer+p_handler->iRxBufferLen), p_data, i_len);
    p_handler->iRxBufferLen += i_len;

    // tlv + data
    if (p_handler->iRxBufferLen < 4)
    {
        return hdlcNum;
    }

    // 遍历找头 最短4字节
    for (ii=0; ii<p_handler->iRxBufferLen - 3; /*ii++*/)
    {
        if (EZ_HDLC_FLAG == p_handler->pRxBuffer[ii])
        {
            //HDLC_HEADER_T *pHdlcHeader = (HDLC_HEADER_T *)(p_handler->pRxBuffer+ii);
            unsigned char TotalLen = p_handler->pRxBuffer[ii+1];

            if (TotalLen < 3) //hdlc tlv
            {
                ii++;
                continue;
            }

            // found hdlc
            if (p_handler->iRxBufferLen>=(ii+TotalLen))
            {
                for (jj=1; jj<TotalLen; jj++)
                {
                    // 有头数据； 跳过此头
                    if (*(p_handler->pRxBuffer+ii+jj) == EZ_HDLC_FLAG)
                    {
                        break;
                    }
                }
                if (jj<TotalLen)
                {
                    ii+=jj;
                    continue;
                }

                if (p_handler->pHdlcCallback)
                {
                    p_handler->pHdlcCallback((p_handler->pRxBuffer+ii), TotalLen, obj);
                }
                hdlcNum++;

                ii+=TotalLen;
            }
            else
            {
                break;
            }
        }
        else
        {
            ii++;
        }
    }

    // 剩余数据移前面
    if (ii>0)
    {
        p_handler->iRxBufferLen -= ii;
        memmove(p_handler->pRxBuffer, p_handler->pRxBuffer+ii, p_handler->iRxBufferLen);
    }

    return hdlcNum;
}

#if 0
//CRC表
static const unsigned char static_signal_control_crc8_table[] =
    {
        0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
        157,195, 33,127,252,162, 64, 30, 95,  1,227,189, 62, 96,130,220,
        35,125,159,193, 66, 28,254,160,225,191, 93,  3,128,222, 60, 98,
        190,224,  2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
        70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89,  7,
        219,133,103, 57,186,228,  6, 88, 25, 71,165,251,120, 38,196,154,
        101, 59,217,135,  4, 90,184,230,167,249, 27, 69,198,152,122, 36,
        248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91,  5,231,185,
        140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
        17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
        175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
        50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
        202,148,118, 40,171,245, 23, 73,  8, 86,180,234,105, 55,213,139,
        87,  9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
        233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
        116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53
    };

//计算crc8
unsigned char ez_crc8(const unsigned char intv,const unsigned char *dat, const unsigned int len)
{
    unsigned char crc=intv;
    unsigned int i;
    for(i=0;i<len;i++)
    {
        crc = crc^dat[i];
        crc = static_signal_control_crc8_table[crc];
    }
    return crc;
}
#endif //0
/*
int main(int argc, char *argv[])
{
    //unsigned char buf[]={0x55, 0x12, 0x34, 0x03, 0x01};
    //unsigned char buf[]={0x01, 0x65, 0x6E, 0x00, 0x01, 0x01, 0x20, 0xB9, 0x00};
    unsigned char buf[] = {0x01, 0x6E, 0x65, 0x00, 0x01, 0x01, 0x20, 0x9D, 0x6B, 0xBF, 0x52, 0x55, 0xC5, 0x0C, 0xC2};
 
    unsigned char CRC;
 
    CRC = signal_control_crc8(0x33, buf, sizeof(buf));
    printf("signal_control_crc8:0x%0x\n", CRC);
 
    // crc结果为aa， 将aa加入运算， 正好为0是正确的
    unsigned char buf_r[] = {0x01, 0x6E, 0x65, 0x00, 0x01, 0x01, 0x20, 0x9D, 0x6B, 0xBF, 0x52, 0x55, 0xC5, 0x0C, 0xC2, 0xAA};
    CRC = signal_control_crc8(0x33, buf_r, sizeof(buf_r));
    printf("signal_control_crc8:0x%0x\n", CRC);
    // 校验数据的时候示例如下：
    if (signal_control_crc8(0x33, buf_r, sizeof(buf_r))==0)
    {
        printf("crc ok\n");
    }
 
    return 0;
}
 
*/


// 发送前
// lenDest >= lenSrc
int ez_hdlc_encode(unsigned char *pDest, int lenDest, unsigned char *pSrc, int lenSrc)
{
    int ii = 0;
    int iDestPos = 0;

    if (lenSrc<=0)
    {
        return 0;
    }

    if (lenSrc>lenDest)
    {
        return -1;
    }

    if (pDest==NULL
        || pSrc == NULL)
    {
        return -2;
    }

    for (ii=0; ii<lenSrc; ii++)
    {
        if (pSrc[ii] == EZ_HDLC_ESC_ASYNC
            || pSrc[ii] == EZ_HDLC_FLAG)
        {
            *(pDest+iDestPos) = EZ_HDLC_ESC_ASYNC;
            iDestPos++;
            if (iDestPos>=lenDest)
            {
                return -3;
            }
            *(pDest+iDestPos)=pSrc[ii]^EZ_HDLC_ESC_COMPL;
            iDestPos++;
            if (iDestPos>=lenDest)
            {
                return -4;
            }
        }
        else
        {
            *(pDest+iDestPos)=pSrc[ii];
            iDestPos++;
            if (iDestPos>=lenDest)
            {
                return -5;
            }
        }

    }

    return iDestPos;
}

// 将hdlc数据， 规范为可用数据
// lenDest >= lenSrc
int ez_hdlc_decode(unsigned char *pDest, int lenDest, unsigned char *pSrc, int lenSrc)
{
    int ii = 0;
    int iDestPos = 0;

    if (lenSrc<=0)
    {
        return 0;
    }

    if (lenDest <= 0)
    {
        return -1;
    }

    if (pDest==NULL
        || pSrc == NULL)
    {
        return -2;
    }

    for (ii=0; ii<lenSrc; ii++)
    {
        if (pSrc[ii] == EZ_HDLC_ESC_ASYNC)
        {
            ii++;
            // 7d标记后面没有数据， 不对
            if (ii>=lenSrc)
            {
                return -3;
            }
            // 连续两个7d， 数据错误
            if (pSrc[ii] == EZ_HDLC_ESC_ASYNC)
            {
                return -4;
            }

            *(pDest+iDestPos)=pSrc[ii]^EZ_HDLC_ESC_COMPL;
            iDestPos++;
            // 缓存不足
            if (iDestPos>=lenDest)
            {
                return def_RET_BUFF_NOT_ENOUGH;
            }

        }
        else
        {
            *(pDest+iDestPos)=pSrc[ii];
            iDestPos++;
            if (iDestPos>=lenDest)
            {
                return -6;
            }
        }
    }

    return iDestPos;
}


// 获取数据包长度
int ez_hdlc_get_dst_buf_len(unsigned char *pSrc, int lenSrc, int Mode)
{
    // todo
    int ii=0;
    int iPacketLen = 0;

    if (lenSrc<=0 || pSrc==NULL)
    {
        return 0;
    }

    for (ii=0; ii<lenSrc; ii++)
    {
        if (pSrc[ii] == EZ_HDLC_FLAG
            ||pSrc[ii] == EZ_HDLC_ESC_ASYNC)
        {
            iPacketLen+=2;
        }
        else
        {
            iPacketLen++;
        }
    }

    if (Mode == EZ_HDLC_MODE_VERIFY)
    {
        iPacketLen++;
    }
    else if (Mode == EZ_HDLC_MODE_VERIFY_MOD_BUS)
    {
        // crc 16
        iPacketLen += 2;
    }
    
    if (iPacketLen+EZ_HDLC_HEAD_LEN > EZ_HDLC_PACKET_MAX_LEN)
    {
        return -1 * ( iPacketLen + EZ_HDLC_HEAD_LEN );
    }

    return iPacketLen+EZ_HDLC_HEAD_LEN/*2*//*EZ_HDLC_FLAG, len*/;
}

// 生成hdlc数据包
// Mode  1, 使用CRC校验码， 2， 不使用校验码
// return -1 输入数据不正确
//		-2 HDLC缓存不足
//		-3 HDLC包超长
//		-5 dest缓存长度不足
// >0  hdlc包长
int ez_hdlc_make_packet(unsigned char *pDestHDLC, int lenDestHDLC, unsigned char *pSrc, int lenSrc, int Mode)
{
    int iRet;
    unsigned char CRC8Value;

    iRet = ez_hdlc_encode(pDestHDLC+EZ_HDLC_HEAD_LEN, lenDestHDLC-EZ_HDLC_HEAD_LEN, pSrc, lenSrc);
    if (iRet < 0)
    {
        return iRet;
    }

    if (Mode == EZ_HDLC_MODE_VERIFY)
    {
        //
        if (iRet>=lenDestHDLC-EZ_HDLC_HEAD_LEN)
        {
            return -2;
        }

        CRC8Value = ez_crc8(0x33, pSrc, lenSrc);

        pDestHDLC[EZ_HDLC_HEAD_LEN+iRet] = CRC8Value;
        iRet ++;
    }
    else if (Mode == EZ_HDLC_MODE_VERIFY_MOD_BUS)
    {
        //
        if (iRet+2>lenDestHDLC-EZ_HDLC_HEAD_LEN)
        {
            return -2;
        }

        unsigned short usret = ez_modbus_crc16((unsigned char *)pSrc, lenSrc);

        pDestHDLC[EZ_HDLC_HEAD_LEN+iRet] = (usret>>8)&0xff;
        pDestHDLC[EZ_HDLC_HEAD_LEN+iRet+1] = usret&0xff;

        // crc 16
        iRet += 2;
    }

    iRet += EZ_HDLC_HEAD_LEN;
    if (iRet>=EZ_HDLC_FLAG)
    {
        //printf("iRet:%d\n", iRet);
        return -3;
    }

    pDestHDLC[0] = EZ_HDLC_FLAG;
    pDestHDLC[1] = iRet;

    return iRet;
}

// 提取数据
// Mode  1, 使用CRC校验码， 2， 不使用校验码
// return -1 输入数据不正确
//		-2 非HDLC数据包
//		-3 HDLC数据校验出错(Mode==1时)
int ez_hdlc_get_data(unsigned char *pDest, int lenDest, unsigned char *pSrcHDLC, int lenSrcHDLC, int Mode)
{
    int iRet = 0;

    if (lenSrcHDLC <= EZ_HDLC_HEAD_LEN)
    {
        return 0;
    }

    // Check if it's a valid HDLC packet (first byte should be EZ_HDLC_FLAG)
    if (pSrcHDLC[0] != EZ_HDLC_FLAG)
    {
        return -2;
    }

    iRet = ez_hdlc_decode(pDest, lenDest, pSrcHDLC+EZ_HDLC_HEAD_LEN,lenSrcHDLC-EZ_HDLC_HEAD_LEN);
    if (iRet < 0)
    {
        return iRet;
    }

    if (Mode == EZ_HDLC_MODE_VERIFY)
    {
        // no crc bytes
        if (lenSrcHDLC <= EZ_HDLC_HEAD_LEN)
        {
            return 0;
        }

        if (ez_crc8(0x33, pDest, iRet) != 0 )
        {
            return -3;
        }

        // ignore crc bytes
        iRet--;
    }
    else if (Mode == EZ_HDLC_MODE_VERIFY_MOD_BUS)
    {
        // crc 16
        iRet -= 2;
    }

    return iRet;
}

#if 0
int main(int argc, char *argv[])
{
    unsigned char snddata[] = {0x7e, 0x1, 0xbc, 0x7d, 0xde, 0x1, 0xe2, 0x0, 0x0, 0x0, 0x3c, 0xbc, 0x7e };
    unsigned char sndbuf[1024]={0};
    unsigned char rcvbuf[1024]={0};

    int ret = 0;
    int ii = 0;

    printf("sizeof(snddata):%d\n", sizeof(snddata));
    printf("snddata:\n");
    for (ii=0; ii<sizeof(snddata); ii++)
    {
        printf("0x%02x ", snddata[ii]);
    }
    printf("\n");

    ret = ez_hdlc_encode(sndbuf, sizeof(sndbuf), snddata, sizeof(snddata));
    printf("hdlc_encode %d:\n", ret);


    printf("sndbuf:\n");
    for (ii=0; ii<ret; ii++)
    {
        printf("0x%02x ", sndbuf[ii]);
    }
    printf("\n");

    ret = ez_hdlc_decode(rcvbuf, sizeof(rcvbuf), sndbuf, ret);
    printf("hdlc_decode %d:\n", ret);
    printf("rcvbuf:\n");
    for (ii=0; ii<ret; ii++)
    {
        printf("0x%02x ", rcvbuf[ii]);
    }
    printf("\n");

    return 0;
}
#endif

