#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ezutil/str_opr.h>

int main(int argc, char *argv[])
{
    {
        const char *src = "instantaneous_ops_per_sec:0";
        int wantRET = 0;
        char left[128] = {0};
        char right[128] = {0};
        const char *spliter = ":";
        size_t src_len = 0;

        int iret = split_str(
            src,
            left,
            right,
            spliter,
            src_len);

        assert(iret == wantRET);
        printf("iret:%d--[%s]:[%s]\n", iret, left, right);
    }
    {
        const char *src = "instantaneous_ops_per_sec:0:a:b";
        int wantRET = 0;
        char left[128] = {0};
        char right[128] = {0};
        const char *spliter = ":";
        size_t src_len = 0;

        int iret = split_str(
            src,
            left,
            right,
            spliter,
            src_len);

        assert(iret == wantRET);
        printf("iret:%d--[%s]:[%s]\n", iret, left, right);
    }
    // 没找到
    {
        const char *src = "instantaneous_ops_per_sec0";
        int wantRET = 0;
        char left[128] = {0};
        char right[128] = {0};
        const char *spliter = ":";
        size_t src_len = 0;

        int iret = split_str(
            src,
            left,
            right,
            spliter,
            src_len);

        assert(iret == wantRET);
        printf("iret:%d--[%s]:[%s]\n", iret, left, right);
    }

    // 没找到
    {
        const char *src = "";
        int wantRET = -1000;
        char left[128] = {0};
        char right[128] = {0};
        const char *spliter = ":";
        size_t src_len = 0;

        int iret = split_str(
            src,
            left,
            right,
            spliter,
            src_len);

        assert(iret == wantRET);
        printf("iret:%d--[%s]:[%s]\n", iret, left, right);
    }

    return 0;
}
