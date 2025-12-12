#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <ezutil/str_opr.h>
#include <ezutil/ez_fs.h>
#include <ezutil/ez_c_printf.h>

#define _defRandomSize 1024

int main(int argc, char *argv[])
{

    const size_t genSize = argc > 1 ? atoi(argv[1]) : 8;   // 生成个数
    const size_t genTimes = argc > 2 ? atoi(argv[2]) : 1;  // 生成次数
    const size_t dumpTimes = argc > 3 ? atoi(argv[3]) : 1; // 生成次数

    printf("Usage:%s [genSize=8]:%ld [genTimes=1]:%ld [dumpTimes=1]:%ld\n", argv[0], genSize, genTimes, dumpTimes);

    size_t i = 0;
    for (i = 0; i < genTimes; i++)
    {
        char randBuf[_defRandomSize] = {0};
        ssize_t iret = ez_rand_buf(randBuf, genSize);

        if (iret <= 0)
        {
            printf("ez_rand_buf failed!\n");
        }
        else
        {
            if (dumpTimes > i)
                dumpBuffer(stdout, (unsigned char *)randBuf, iret,
                           SHOW_ASCII | SHOW_BINAR | SHOW_HEXAD | SHOW_LINER);
        }
    }

    return (0);
}

