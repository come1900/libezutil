/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * crc.h - src/library/libezutil
 *
 * $Id: crc.h 5884 2008-7-16 11:33:42Z wu_junjie $
 *
 *  Explain:
 *     crc16/crc32
 *
 *  Update:
 *     2008-7-16 11:33:24 WuJunjie 10221 Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef _dataIdx_H
#define _dataIdx_H

typedef struct __dataIdx_t
{
	unsigned short id;			// espid
	unsigned short tempreture;	//温度
	unsigned long lastUpdateMs; //更新时间
} dataIdx_t;

#ifdef __cplusplus
extern "C"
{
#endif

	int existId(dataIdx_t *tContainer, const unsigned short Id);
	int updateData(dataIdx_t *tContainer, const dataIdx_t *pData);

#ifdef __cplusplus
}
#endif

#endif //_dataIdx_H

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/* USAGE
	//--------------------------
	printf("orig:%s\n", pOrig);
	usret = ez_crc16(0x0, (unsigned char *)pOrig, strlen(pOrig));
	printf("ez_crc16:0x%0X\n", usret);

	//--------------------------
	printf("orig:%s\n", pOrig);
	uiret = ez_crc32((unsigned char *)pOrig, strlen(pOrig));
	printf("ez_crc32:0x%0X\n", uiret);
	printf("this crc32 alo is:%s\n", uiret==0x5CFCCFF6?"OK":"NOK");
*/
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
