/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * crc.c - /home/wujj/src/libs/tutorial_ezutil
 *
 * Copyright (C) 2008 ezlibs.com, All Rights Reserved.
 *
 * $Id: crc.c 5884 2008-7-14 11:00:42Z wu_junjie $
 *
 *  Explain:
 *     crc user
 *
 *  Update:
 *     2008-7-14 11:00:24 WuJunjie 10221 Create
 *
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ezutil/str_opr.h>
#include <ezutil/crc.h>


int main (int argc, char **argv)
{
	unsigned int uiret;
	unsigned short usret;
	const char * const pOrig = "hellostr";

	//--------------------------
	printf("orig:%s\n", pOrig);
	usret = ez_crc16(0x0, (unsigned char *)pOrig, strlen(pOrig));
	printf("ez_crc16:0x%0X\n", usret);

	//--------------------------
	unsigned char buf[]={0x01 , 0x03 , 0x00 , 0x00 , 0x00 , 0x02};
	printf("orig:0x%0x ..., len:%ld\n", buf[0], sizeof(buf));
	usret = ez_modbus_crc16((unsigned char *)buf, sizeof(buf));
	// CRC: 高字节在前，低字节在后, PKTH-100B 直接取用， 即：c40b
    printf("CRC:0x%0x, -- 0x%0x, 0x%0x\n", usret, usret&0xff/**/, (usret>>8)&0xff);
	//--------------------------
	printf("orig:%s\n", pOrig);
	uiret = ez_crc32((unsigned char *)pOrig, strlen(pOrig));
	printf("ez_crc32:0x%0X\n", uiret);
	printf("this crc32 alo is:%s\n", uiret==0x5CFCCFF6?"OK":"NOK");

	return 0;
}
