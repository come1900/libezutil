/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * t-ez_crc16_xmoden.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: t-ez_crc16_xmoden.c 5884 2022-07-01 09:43:18Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2022-07-01 09:43:18  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
///////////////
//测试数据
// 5A000102FF0000885A
unsigned char test_buff[] = {
    0x00, 0x01, 0x02, 0xFF, 0x00, 0x00};

#include <stdio.h>
#include <stdlib.h>

#include "crc.h"

int main(int argc, char *argv[])
{
    unsigned short crc;

    //查表法
    crc = ez_crc16_crc16_xmoden(test_buff, sizeof(test_buff));

    printf("CRC-16/XMODEM : 0x%x (%s)\n", crc, crc == 0x885a ? "OK" : "FAILED");

    return 0;
}