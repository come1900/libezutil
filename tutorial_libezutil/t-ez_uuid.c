
#include <stdio.h>
#include <stdlib.h>

#include <ezutil/str_opr.h>

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    char uuid_buf[def_ez_uuid_len + 1] = {0};
    char *pBuf = NULL;

    /* 测试 1：使用内部静态缓冲区（dst_uuid == NULL, ui_seed == 0） */
    pBuf = ez_uuid(NULL, 0, NULL);
    printf("%s:", "ez_uuid(NULL, 0, NULL)");
    if (pBuf == NULL)
    {
        printf("%s\n", "failed!");
    }
    else
    {
        printf("%s\n", pBuf);
    }

    /* 测试 2：使用外部缓冲区（指定 dst_uuid），ui_seed == 0 */
    pBuf = ez_uuid(uuid_buf, 0, NULL);
    printf("%s:", "ez_uuid(buf, 0, NULL)");
    if (pBuf == NULL)
    {
        printf("%s\n", "failed!");
    }
    else
    {
        printf("%s\n", uuid_buf);
    }

    /* 测试 3：使用不同 ui_seed，观察输出是否变化 */
    pBuf = ez_uuid(uuid_buf, 12345, "seed123");
    printf("%s:", "ez_uuid(buf, 12345, \"seed123\")");
    if (pBuf == NULL)
    {
        printf("%s\n", "failed!");
    }
    else
    {
        printf("%s\n", uuid_buf);
    }

    return 0;
}


