/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ez_log.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ez_log.c 5884 2015-4-9 9:30:32Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2015-4-9 9:30:40 WuJunjie Create
 *     2015-04-23 02:17:53 WuJunjie Modify, level control
 *     2015-6-24 10:29:14 WuJunjie Modify, log without datetime
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#include <time.h>
//#include <sys/timeb.h>
#include <stdarg.h>
#ifdef WIN32
    #include <windows.h>
    #include <io.h>
#else
    #include <unistd.h>
    #include <sys/time.h>
    #include <pthread.h>
    #define  CRITICAL_SECTION   pthread_mutex_t
    #define  _vsnprintf         vsnprintf
#endif

#include "ez_log.h"

//Log{
#define EZ_LOG_MAX_LINE_SIZE 1024*128
//#define EZ_LOG_MAX_LINE_SIZE 1024*1024*2

// 文件名缓存
static char logfilename1[256];
static char logfilename2[256];
static char logfilename3[256];
// 可以任意大
static int log_nums=2;
static int log_level = EZ_LOG_LEVEL_NONE;
static int log_to = EZ_LOG_TO_STDOUT;
// bytes
static int log_size = 0;// = EZ_LOG_DEFAULT_FILE_SIZE;

static char logstr[EZ_LOG_MAX_LINE_SIZE+1];
static char datestr[16];
static char timestr[16];
static char mss_str[3+4+1];

static CRITICAL_SECTION cs_log;
static FILE *flog = NULL;


#ifdef WIN32
void Lock(CRITICAL_SECTION *l)
{
    EnterCriticalSection(l);
}
void Unlock(CRITICAL_SECTION *l)
{
    LeaveCriticalSection(l);
}
#else
void Lock(CRITICAL_SECTION *l)
{
    pthread_mutex_lock(l);
}
void Unlock(CRITICAL_SECTION *l)
{
    pthread_mutex_unlock(l);
}
#endif

int mk_log(int logLevel, const char *pszFmt, va_list argp)
{
    struct tm *now;
    time_t time_sec_utc;
    unsigned short millitm;

    if (NULL==pszFmt || 0==pszFmt[0])
    {
        return -1;
    }

    if (logLevel > EZ_LOG_LEVEL_NONE
        || logLevel<EZ_LOG_LEVEL_EMERG)
    {
        return -2;
    }

    _vsnprintf(logstr,EZ_LOG_MAX_LINE_SIZE,pszFmt,argp);

    struct timespec tv;
    clock_gettime(CLOCK_REALTIME_COARSE, &tv);

    time_sec_utc = tv.tv_sec;
    millitm = (unsigned short)( ((tv.tv_nsec/1000) + 500) / 1000 );

    if (millitm >= 1000) {
        ++time_sec_utc;
        millitm -= 1000;
    }
    //tb->timezone = tz.tz_minuteswest;
    //tb->dstflag  = tz.tz_dsttime;

    //ftime(&tb);
    now=localtime(&time_sec_utc);
    sprintf(datestr,"%04d-%02d-%02d", now->tm_year+1900,now->tm_mon+1,now->tm_mday);
    sprintf(timestr,"%02d:%02d:%02d", now->tm_hour     ,now->tm_min  ,now->tm_sec );
    sprintf(mss_str,"%03d -%1d-", millitm, logLevel);
    //sprintf(mss_str,"%03d", tb.millitm);

    return 0;
}

// iFmt :
//		0 - logstr only
//		ff - logstr all
void log_file(int iFmt)
{
    int ii;
    int iret;

    if (flog==NULL && logfilename1[0] != '\0')
    {
        flog=fopen(logfilename1, "a");
    }
    else
    {
        //flog = NULL;
    }

    if (NULL != flog)
    {
    	if (0==iFmt)
		{
    	    iret = fprintf(flog, " %s", logstr);
		}
		else
		{
    	    iret = fprintf(flog, "%s %s.%s %s",datestr,timestr,mss_str,logstr);
		}

		if (iret<0)
        {
            fclose(flog);
            flog = NULL;
            return;
        }

        if (ftell(flog)>log_size)
        {
            // 先关掉
            fclose(flog);
            flog = NULL;

            sprintf(logfilename3, "%s%d", logfilename1, log_nums);
			strcpy(logfilename2, logfilename3);
            for (ii=log_nums-1; ii>0; ii--)
            {
                sprintf(logfilename2, "%s%d", logfilename1, ii);
                //printf("rename:%s %s\n", logfilename2,logfilename3);
                if (rename(logfilename2,logfilename3))
                {
                    remove (logfilename3);

                    rename(logfilename2, logfilename3);
                }
                strcpy(logfilename3, logfilename2);
            }

            if (rename(logfilename1,logfilename2))
            {
                remove (logfilename2);

                rename(logfilename1, logfilename2);
            }

        }
        else
        {}
    }
    else
    {
        fprintf(stdout, "~!: %s %s.%s %s",datestr,timestr,mss_str,logstr);
    }

}

int ez_log_structure(const char *f1, const unsigned int fnum, const unsigned int flen)
{
    if (f1 != NULL
        && f1[0]!='\0')
    {
        strcpy(logfilename1, f1);
    }
    else
    {
        strcpy(logfilename1, "ezlogs.log");
    }

    if (fnum < 1)
    {
        log_nums = 1;
    }
    else
    {
        log_nums = fnum;
    }
    // bytes
    log_size = flen;

#ifdef WIN32
    InitializeCriticalSection(&cs_log);
#else
    pthread_mutex_init(&cs_log,NULL);
#endif

    return 0;
}

int ez_log_destructor()
{
    if (flog)
    {
        fclose(flog);
        flog = NULL;
    }

#ifdef WIN32
    DeleteCriticalSection(&cs_log);
#else

    pthread_mutex_destroy(&cs_log);
#endif

    return 0;
}

int ez_log_set_level(int level)
{
    int __level = level%10;
    int __to = level/10;
    if (__level > EZ_LOG_LEVEL_NONE
        || __level<EZ_LOG_LEVEL_EMERG)
    {
        return -1;
    }
    if (__to > EZ_LOG_TO_FILE_AND_STDOUT
        || __to < EZ_LOG_TO_STDOUT)
    {
        return -2;
    }

    log_level = __level;
    log_to = __to;

    return 0;
}

void ez_log_L(int level, const char *pszFmt,...)
{
    int ii;

    if (level > log_level)
    {
        return ;
    }

    va_list argp;

    Lock(&cs_log);
    va_start(argp, pszFmt);

    ii = mk_log(level, pszFmt, argp);
    va_end(argp);

    if (ii<0)
    {
        Unlock(&cs_log);
        return;
    }

    if (log_to==EZ_LOG_TO_FILE || log_to==EZ_LOG_TO_FILE_AND_STDOUT)
    {
        log_file(0xff);
    }

    if (log_to==EZ_LOG_TO_STDOUT || log_to==EZ_LOG_TO_FILE_AND_STDOUT)
    {
        fprintf(stdout, "%s %s.%s %s",datestr,timestr,mss_str,logstr);
    }

    Unlock(&cs_log);
}

void ez_log_cnt_L(int level, const char *pszFmt,...)
{
    int ii;

    if (level > log_level)
    {
        return ;
    }

    va_list argp;

    Lock(&cs_log);
    va_start(argp, pszFmt);

    ii = mk_log(level, pszFmt, argp);
    va_end(argp);

    if (ii<0)
    {
        Unlock(&cs_log);
        return;
    }

    if (log_to==EZ_LOG_TO_FILE || log_to==EZ_LOG_TO_FILE_AND_STDOUT)
    {
        log_file(0);
    }

    if (log_to==EZ_LOG_TO_STDOUT || log_to==EZ_LOG_TO_FILE_AND_STDOUT)
    {
        fprintf(stdout, "%s",logstr);
    }

    Unlock(&cs_log);
}

void ez_log(const char *pszFmt,...)
{
    va_list argp;
    int ii;

    Lock(&cs_log);
    va_start(argp, pszFmt);
    ii = mk_log(EZ_LOG_LEVEL_INFO, pszFmt, argp);
    va_end(argp);

    if (ii<0)
    {
        Unlock(&cs_log);
        return;
    }
    log_file(0xff);

    Unlock(&cs_log);
}

