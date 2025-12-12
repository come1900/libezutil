/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ez_hdlc.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ez_hdlc.h 5884 2015-03-04 05:03:01Z WuJunjie $
 *
 *  Explain:
 *     -
 *      具有数据起始标识的封装， 用于容错
 *      数据长度最大 EZ_HDLC_PACKET_MAX_LEN
 *     -
 *
 *  Update:
 *     2015-03-04 05:03:01  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#if !defined(_ez_hdlc_H)
#define _ez_hdlc_H

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
//#define EZ_HDLC_UART_BITRATE			115200	// uart bitrate
#define EZ_HDLC_FLAG         			0xFE    // HDLC Head flag
//#define EZ_HDLC_VER         			0x01    // HDLC async start/stop flag
#define EZ_HDLC_HEAD_LEN       			0x02    // tl
#define EZ_HDLC_ESC_ASYNC	  			0x7D    // HDLC transparency escape flag for async
#define EZ_HDLC_ESC_COMPL	  			0x20    // HDLC transparency bit complement mask

#define EZ_HDLC_PACKET_MAX_LEN       	0xFD    //
//校验方式
#define EZ_HDLC_MODE_VERIFY        			1    // HDLC with crc8
#define EZ_HDLC_MODE_VERIFY_MOD_BUS         2    // HDLC with crc16
#define EZ_HDLC_MODE_NOT_VERIFY    			3    // HDLC
#define EZ_HDLC_MODE_DEF        			EZ_HDLC_MODE_VERIFY    // HDLC Head flag

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
// <= EZ_HDLC_PACKET_MAX_LEN, can set more little for lower mem sys
#define EZ_HDLC_BUFF_LEN EZ_HDLC_PACKET_MAX_LEN // max 0xfd = 253

// do not need tx buf for normal use.
//#define EZ_HDLC_TX_BUFF_LEN 0//EZ_HDLC_BUFF_LEN*2

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#define def_RET_BUFF_NOT_ENOUGH -5

#ifdef __cplusplus
extern "C"
{
#endif

    typedef int (CALLBACK_ON_HDLC) (unsigned char *buf, int len, void *arg);
    //typedef int (CALLBACK_OBJ_ON_HDLC) (void *obj, unsigned char *buf, int len);
    typedef struct __hdlc_handler_t
    {
        int iInterface; // 接口编号
        unsigned char *pRxBuffer; // 接收缓存
        unsigned int iRxBufferLen; // 接收缓存数据长度
#if defined(EZ_HDLC_TX_BUFF_LEN) && EZ_HDLC_TX_BUFF_LEN>0

        unsigned char *pTxBuffer; // 发送缓存
        unsigned int iTxBufferLen; // 发送缓存数据长度
#endif //EZ_HDLC_TX_BUFF_LEN

        CALLBACK_ON_HDLC *pHdlcCallback;

        /* A pointer to get hook to the "connection" or "socket" object */
        void *obj;
    }
    HDLC_HANDLER_T;

    HDLC_HANDLER_T *hdlc_new();
    int hdlc_ondata(HDLC_HANDLER_T *p_handler, unsigned char *p_data, unsigned int i_len, void *obj);
    int hdlc_delete(HDLC_HANDLER_T *p_handler);

    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    unsigned char ez_crc8(const unsigned char intv, const unsigned char *dat, const unsigned int len);

    // 发送前
    // lenDest >= lenSrc
    int ez_hdlc_encode(unsigned char *pDest, int lenDest, unsigned char *pSrc, int lenSrc);
    // 将hdlc数据， 规范为可用数据
    // lenDest >= lenSrc
    int ez_hdlc_decode(unsigned char *pDest, int lenDest, unsigned char *pSrc, int lenSrc);

    // 获取数据包长度, (转换好的)数据长度+3
    int ez_hdlc_get_dst_buf_len(unsigned char *pSrc, int lenSrc, int Mode);
    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    // 生成hdlc数据包
    // Mode  1, 使用CRC校验码， 2， 不使用校验码
    // return -1 输入数据不正确
    //		-2 HDLC缓存不足
    //		-3 HDLC包超长
    int ez_hdlc_make_packet(unsigned char *pDestHDLC, int lenDestHDLC, unsigned char *pSrc, int lenSrc, int Mode);
    // 提取数据
    // Mode  EZ_HDLC_MODE_VERIFY, 使用CRC校验码， EZ_HDLC_MODE_NOT_VERIFY， 不使用校验码
    // return -1 输入数据不正确
    //		-2 非HDLC数据包
    //		-3 HDLC数据校验出错(Mode==1时)
    int ez_hdlc_get_data(unsigned char *pDest, int lenDest, unsigned char *pSrcHDLC, int lenSrcHDLC, int Mode);

#ifdef __cplusplus
}
#endif

#endif // !defined(_ez_hdlc_H)
