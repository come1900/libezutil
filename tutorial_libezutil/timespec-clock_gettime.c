/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * timespec-clock_gettime.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: timespec-clock_gettime.c 5884 2021-11-17 08:40:12Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2021-11-17 08:40:12  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include<unistd.h> // usleep
#include <ezutil/timespec.h>

int main(int argc,char * argv[])
{
    struct timespec ts1;
    struct timespec ts2;

    // CLOCK_MONOTONIC : 系统启动后流逝的时间，由变量jiffies来记录的
    // CLOCK_REALTIME：相对时间，从1970.1.1到目前的时间
    // CLOCK_REALTIME_COARSE，CLOCK_MONOTONIC_COARSE：带COARSE后缀的，精度没有不带后缀的高，但是速度快。
    clock_gettime(CLOCK_MONOTONIC, &ts1);
    usleep(1000);
    clock_gettime(CLOCK_MONOTONIC, &ts2);

    printf("%32s : %-8ld, %ld\n", "ts1", ts1.tv_sec, ts1.tv_nsec);
    printf("%32s : %-8ld, %ld\n", "ts2", ts2.tv_sec, ts2.tv_nsec);
    printf("%32s : %f\n", "timespec_to_double", timespec_to_double(ts2));
    printf("%32s : %ld\n", "timespec_to_ms",    timespec_to_ms(ts2));
    printf("%32s : %ld\n", "timespec_sub(ms)",  timespec_to_ms(timespec_sub(ts2, ts1)) );
    printf("%32s : %d\n", "timespec_gt(ts2, ts1)", timespec_gt(ts2, ts1) );

    return 0;
}
