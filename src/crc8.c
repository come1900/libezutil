/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * crc16.c - src/library/libezutil
 *
 * $Id: crc16.c 5884 2008-7-16 11:33:42Z wu_junjie $
 *
 *  Explain:
 *     -crc16-
 *
 *  Update:
 *     2008-7-16 11:33:24 WuJunjie 10221 Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include "crc.h"

//CRC表
static const unsigned char const static_signal_control_crc8_table[] =
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