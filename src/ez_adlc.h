/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ez_adlc.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ez_adlc.h 5884 2022-07-13 08:26:09Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2022-07-13 08:26:09  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef _ez_adlc_H
#define _ez_adlc_H

// A-level Data Link Control，A级数据链路控制
//数据包到达回调
// 返回值:
//     -1: 不正常协议包， 即包括下述不正常， 不想区分错误细节的时候可以直接返回-1
//     -2: 没有标志位
//     -3: 长度不足
//      0: 此包正常
//     >0: 表示正常已用的buf数据长度
typedef int(CALLBACK_ON_ADLC)(unsigned char *buf, unsigned int len, void *arg);

typedef struct __adlc_handler_t
{
	unsigned char *rxBuf;  // 接收缓存
	unsigned int rxBufLen; // 接收缓存数据长度

	unsigned char *execBuf;	 // 拼接用缓存
	unsigned int execBufLen; // 拼接长度

	CALLBACK_ON_ADLC *pCbAdlc;

	// private:
	// 每数据帧最大长度
	unsigned int iMaxPacketLen;
	unsigned char headFlag;

} ADLC_HANDLER_T;

#ifdef __cplusplus
extern "C"
{
#endif

	ADLC_HANDLER_T *adlc_create(const unsigned int maxPacketLen, const unsigned char packetFlag, CALLBACK_ON_ADLC * pOnAdlc);
	int adlc_destroy(ADLC_HANDLER_T *pHandler);
	int adlc_ondata(ADLC_HANDLER_T *pHandler, unsigned char *pData, unsigned int dataLen);

#ifdef __cplusplus
}
#endif

#endif //_ez_adlc_H

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/* USAGE
	//--------------------------

*/
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
