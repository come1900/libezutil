/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * u-ez_system_api.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: u-ez_system_api.c 5884 2021-07-30 04:52:07Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2021-07-30 04:52:07  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <ezutil/ez_system_api.h>

int main(int argc, char *argv[])
{
    printf("%16s:%ld\n%16s:%ld\n", "time", time(NULL), "ez_time_ms", ez_time_ms(NULL));

    return 0;
}
