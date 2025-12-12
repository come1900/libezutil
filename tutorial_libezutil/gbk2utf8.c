//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ezutil/str_opr.h>

int main(int argc, char *argv[])
{
#if defined(HAVE_ICONV)
    //char *argChannelName = "好的hao";
    char *argChannelName = "测试设备48";//\346\265\213\350\257\225\350\256\276\345\244\20748
    unsigned char buffSize[256] = {'\0'};

    printf("%s:%s\n", "argChannelName", argChannelName);
    dumpBuffer(stdout, (unsigned char *)argChannelName,
               strlen(argChannelName),
               SHOW_ASCII | SHOW_BINAR | SHOW_HEXAD | SHOW_LINER);

    int iret_ez_gbk_to_utf8 = ez_gbk_to_utf8((char *)buffSize, sizeof(buffSize)-1, argChannelName);

    if (0==iret_ez_gbk_to_utf8)
    {
        //argChannelName = buffSize;

        printf("%s:%s\n", "buffSize", buffSize);

        dumpBuffer(stdout, (unsigned char *)buffSize,
                   strlen((char *)buffSize),
                   SHOW_ASCII | SHOW_BINAR | SHOW_HEXAD | SHOW_LINER);

        if (buffSize[0] == 0346)
        {
            printf("OK -- %o\n", buffSize[0]);
        }
        else
        {
            printf("NOK -- %o\n", buffSize[0]);
        }

    }
    else
    {
        printf("%s:%d\n", "iret_ez_gbk_to_utf8", iret_ez_gbk_to_utf8);
    }
#else
    printf("%s\n", "HAVE_ICONV not defined.");
#endif
    return 0;
}
