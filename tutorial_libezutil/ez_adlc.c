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

#include <ezutil/str_opr.h>

#include "ez_adlc.h"

#define ARG_USED(x) (void)&x;


// 、地址、控制、信息和帧校验

#define GS_ADLC_FLAG 0x5A         // ADLC Head flag
#define GS_ADLC_PACKET_MAX_LEN 32 //

typedef struct __ez_gs_adlc_t
{
    unsigned char flag;
    unsigned char len;
    unsigned char cmd;
    // unsigned char verify_H;
    // unsigned char verify_L;
    /* data */
}ez_gs_adlc_t;

int onGsAdlc(unsigned char *buf, unsigned int buflen, void *arg)
{
    // adlc data not valid
    if (NULL == buf || buflen < sizeof (ez_gs_adlc_t) || buf[0] != GS_ADLC_FLAG)
    {
        return -1;
    }

    printf("%s:%u\n", __FUNCTION__, buflen);

    // dumpBuffer(stdout, (unsigned char *)buf,
    //            len,
    //            SHOW_ASCII | SHOW_BINAR | SHOW_HEXAD | SHOW_LINER);

    return 0;
}

int main(int argc, char *argv[])
{
    int iret = 0;

    // 输出28 5 6 长度的三包
    {
        unsigned char packet[] = {0x5a, 0x00, 0x01, 0x12, 0x00, 0x00, 0x13, 0x00, 0x02, 0x06, 0x00, 0x08, 0x00, 0x01, 0x01, 0x0c, 0x02, 0x00, 0x12, 0x0c, 0x92, 0x08, 0x00, 0x0e, 0x04, 0xde, 0x7c, 0x3c};
        ARG_USED(packet);

        printf("%s:%ld\n", "Adlc", sizeof(packet));

        dumpBuffer(stdout, (unsigned char *)packet,
                   sizeof(packet),
                   SHOW_ASCII | SHOW_BINAR | SHOW_HEXAD | SHOW_LINER);

        ADLC_HANDLER_T *pAdlcHandler = adlc_create(GS_ADLC_PACKET_MAX_LEN, GS_ADLC_FLAG, onGsAdlc);
        ARG_USED(pAdlcHandler);

        // 整包
        // 28
        iret = adlc_ondata(pAdlcHandler, packet, sizeof(packet));

        // 垃圾数据
        iret = adlc_ondata(pAdlcHandler, packet + 1, 1);

        // 整包 5
        iret = adlc_ondata(pAdlcHandler, packet, 5);

        // 分包 6
        iret = adlc_ondata(pAdlcHandler, packet, 1);
        iret = adlc_ondata(pAdlcHandler, packet + 1, 5);

        iret = adlc_destroy(pAdlcHandler);
        ARG_USED(iret);
    }

    // 输出3 长度的包
    {
        unsigned char packet[] = {0x00, 0x01, 0x12, 0x5a, 0x00, 0x13, };
        ARG_USED(packet);

        printf("%s:%ld\n", "Adlc", sizeof(packet));

        dumpBuffer(stdout, (unsigned char *)packet,
                   sizeof(packet),
                   SHOW_ASCII | SHOW_BINAR | SHOW_HEXAD | SHOW_LINER);

        ADLC_HANDLER_T *pAdlcHandler = adlc_create(GS_ADLC_PACKET_MAX_LEN, GS_ADLC_FLAG, onGsAdlc);
        ARG_USED(pAdlcHandler);

        // 整包
        iret = adlc_ondata(pAdlcHandler, packet, sizeof(packet));

        iret = adlc_destroy(pAdlcHandler);
        ARG_USED(iret);
    }
    return 0;
}

void t_malloc_free()
{
	unsigned int ii=0;

    for (ii = 1; ii >= 0; ii++)
    {
        ADLC_HANDLER_T *pAdlcHandler = adlc_create(GS_ADLC_PACKET_MAX_LEN, GS_ADLC_FLAG, onGsAdlc);
        ARG_USED(pAdlcHandler);

        int iret = adlc_destroy(pAdlcHandler);

        if (iret < 0)
        {
            printf("adlc_destroy failed.\n");
            exit(-1);
        }

        pAdlcHandler = NULL;

        if (0 == (ii % 999999))
        {
            printf("Cur:%u\n", ii);
        }
    }
}

