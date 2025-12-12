#include <stdio.h>
#include <stdlib.h>

#include "ezutil/str_opr.h"

int main(int argc, char *argv[])
{
    int iret;
    time_t ttNow = time(NULL);
    char bufTimestring[64];
    char *pTimestring_user = "2016-10-07 0:0:0";
    int i_attach_days=argc>1?atoi(argv[1]):0;

    printf("ttNow:%lu, attach_days:%d\n", ttNow, i_attach_days);

    time_t tt_attach_seconds = i_attach_days*24*3600;

    iret = date_time2string(ttNow, bufTimestring, NULL);
    if (iret==0)
    {
        printf("date_time2string ttNow:%lu -> %s\n", ttNow, bufTimestring);
    }
    iret = date2string(ttNow, bufTimestring, NULL);
    if (iret==0)
    {
        printf("date2string ttNow:%lu -> %s\n", ttNow, bufTimestring);
    }
    iret = time2string(ttNow, bufTimestring, NULL);
    if (iret==0)
    {
        printf("time2string ttNow:%lu -> %s\n", ttNow, bufTimestring);
    }

    iret = date_time2string(ttNow+tt_attach_seconds, bufTimestring, NULL);
    if (iret==0)
    {
        printf("date_time2string ttNew:%lu -> %s\n", ttNow+tt_attach_seconds, bufTimestring);
    }
    time_t tt_now_atta = string2date_time(bufTimestring, NULL);
    printf("string2date_time:%s -> %lu\n", bufTimestring, tt_now_atta);
    printf("* Result:%s\n", tt_now_atta==(ttNow+tt_attach_seconds)?"OK":"NOK");


    time_t tt_user = string2date_time(pTimestring_user, NULL);
    printf("string2date_time:%s -> %lu\n", pTimestring_user, tt_user);

    //非标格式测试
    {
        ttNow = time(NULL);
        char *pFormat = "%04d%02d%02d_%02d%02d%02d";

        iret = date_time2string(ttNow, bufTimestring, pFormat);
        if (iret==0)
        {
            printf("date_time2string ttNow:%lu -> %s\n", ttNow, bufTimestring);
        }
        else
        {
            printf("date_time2string failed:%d\n", iret);
        }

        time_t tt_user = string2date_time(bufTimestring, pFormat);
        printf("string2date_time:%s -> %lu\n", bufTimestring, tt_user);
        
        printf("* Result:%s\n", tt_user==ttNow?"OK":"NOK");
    }

    return 0;
}
