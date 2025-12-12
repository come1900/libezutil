
#include <stdio.h>
#include <stdlib.h>

#include <ezutil/str_opr.h>


int main(int argc, char *argv[])
{
	// 获取时间字符串
    char *pBuf;
    char strBuff[GET_DATE_TIME_STRING_DEFAULT_STR_LEN];
    pBuf = get_date_time_string(strBuff, NULL);

    printf("%s:", "get_date_time_string");

    if (pBuf == NULL)
    {
        printf("%s\n", "failed!");
    }
    else
    {
        printf("%s\n", strBuff);
    }

    return 0;
}
