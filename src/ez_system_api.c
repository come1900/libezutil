/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ez_system_api.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ez_system_api.c 5884 2012-11-01 10:40:24Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-11-01 10:40:24  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include "str_opr.h"

#ifdef _WIN32
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")
#else // _WIN32
#include <sys/select.h>/*select() posix 2001*/
#endif

#include "ez_system_api.h"
//#include <sys/time.h>


/* 封装 usleep 功能，使用 nanosleep 实现 */
void ez_usleep(unsigned int usec) {
	struct timespec ts;
	ts.tv_sec = usec / 1000000;
	ts.tv_nsec = (usec % 1000000) * 1000;
	nanosleep(&ts, NULL);
}

int ez_sleep(int second, int microsecond)
{
#ifdef _WIN32
    Sleep(second*1000+microsecond/1000);
#else

    struct timeval t_timeval;
    t_timeval.tv_sec = second;
    t_timeval.tv_usec = microsecond;
    select(0, NULL, NULL, NULL, &t_timeval);
#endif

    return 0;
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

int ez_log2file(char *pFilePathName, char *pLog, char *pMod)
{
    char dtstring[GET_DATE_TIME_STRING_DEFAULT_STR_LEN];

    FILE * pFile;
    pFile = fopen (pFilePathName, pMod);
    if (pFile==NULL)
    {
        printf("Open:%s Failed.\n", pFilePathName);
        fclose (pFile);
        return -1;
    }

    get_date_time_string(dtstring, NULL);

    fprintf (pFile, "ez_libs:::%s\t%s\n", dtstring, pLog );
    fclose (pFile);

    return 0;
}
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

int ez_timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y)
{
    //#ifdef LINUX
    /* Perform the carry for the later subtraction by updating y. */
    if (x->tv_usec < y->tv_usec)
    {
        int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
        y->tv_usec -= 1000000 * nsec;
        y->tv_sec += nsec;
    }
    if (x->tv_usec - y->tv_usec > 1000000)
    {
        int nsec = (x->tv_usec - y->tv_usec) / 1000000;
        y->tv_usec += 1000000 * nsec;
        y->tv_sec -= nsec;
    }

    /* Compute the time remaining to wait.
       tv_usec is certainly positive. */
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;

    /* Return 1 if result is negative. */
    return x->tv_sec < y->tv_sec;
    //#else
    //	return -1;
    //#endif
}

time_t ez_time_ms(time_t *t)
{
    struct timeb tb;
    ftime(&tb);

    if (t != NULL)
        *t = 1000 * tb.time + tb.millitm;

    return 1000 * tb.time + tb.millitm;
}
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
