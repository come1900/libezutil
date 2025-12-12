/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ez_log.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ez_log.h 5884 2015-4-9 9:31:00Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2015-4-9 9:31:05 WuJunjie Create
 *     2015-04-23 02:17:53 WuJunjie Modify, level control
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef _ez_log_H
#define _ez_log_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EZ_LOG_LEVEL_EMERG   0   //系统不可用
#define EZ_LOG_LEVEL_ALERT   1   //必须马上采取行动的事件
#define EZ_LOG_LEVEL_CRIT    2   //关键的事件
#define EZ_LOG_LEVEL_ERR     3   //错误事件
#define EZ_LOG_LEVEL_WARNING 4   //警告事件
#define EZ_LOG_LEVEL_NOTICE  5   //普通但重要的事件
#define EZ_LOG_LEVEL_INFO    6   //有用的信息
#define EZ_LOG_LEVEL_DEBUG   7   //调试信息
#define EZ_LOG_LEVEL_NONE    8   //不记日志

#define EZ_LOG_TO_STDOUT            1 //输出到标准输出
#define EZ_LOG_TO_FILE              2 //输出到文件
#define EZ_LOG_TO_FILE_AND_STDOUT   3 //输出到文件

#define EZ_LOG_DEFAULT_FILE_SIZE    1024*1024

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#ifdef __cplusplus
extern "C"
{
#endif
    // fname: file path name
    // fnum: bak file number
    // flen: (per) file max length
    int ez_log_structure(const char *fname, const unsigned int fnum, const unsigned int flen);
    // Tens:EZ_LOG_TO... digits:EZ_LOG_LEVEL...
    // such as 26, EZ_LOG_TO_FILE and EZ_LOG_LEVEL_INFO
    int ez_log_set_level(int level);
    // date + time + content
    void ez_log_L(int level, const char *pszFmt,...);
    // content only
    void ez_log_cnt_L(int level, const char *pszFmt,...);
    // info level, date + time + content
    // tofile only
    void ez_log(const char *pszFmt,...);
    // release
    int ez_log_destructor();

#ifdef __cplusplus
}
#endif

// for your case
#if defined(_USE_EZLOG)
    #define EZ_LOGS(x) x
    // do not use any more
    #define EZ_LOGS_INFO(x) x
    // do not use any more
    #define EZ_LOGS_WARN(x) x
    // do not use any more
    #define EZ_LOGS_ERROR(x) x

    #define ez_printf_null(...) ez_log_L(EZ_LOG_LEVEL_NONE, __VA_ARGS__)
    #define ez_printf_debug(...) ez_log_L(EZ_LOG_LEVEL_DEBUG, __VA_ARGS__)
    #define ez_printf_info(...) ez_log_L(EZ_LOG_LEVEL_INFO, __VA_ARGS__)
    #define ez_printf_info_content(...) ez_log_cnt_L(EZ_LOG_LEVEL_INFO, __VA_ARGS__)
    #define ez_printf_notice(...) ez_log_L(EZ_LOG_LEVEL_NOTICE, __VA_ARGS__)
    #define ez_printf_warning(...) ez_log_L(EZ_LOG_LEVEL_WARNING, __VA_ARGS__)
    #define ez_printf_error(...) ez_log_L(EZ_LOG_LEVEL_ERR, __VA_ARGS__)
    #define ez_printf_crit(...) ez_log_L(EZ_LOG_LEVEL_CRIT, __VA_ARGS__)
    #define ez_printf_alert(...) ez_log_L(EZ_LOG_LEVEL_ALERT, __VA_ARGS__)
    #define ez_printf_emerg(...) ez_log_L(EZ_LOG_LEVEL_EMERG, __VA_ARGS__)

    #define ez_printf_file(...) ez_log(__VA_ARGS__)
    #define printf ez_printf_debug
#elif defined(_USE_EZLOG_CLASSIC)
    #define EZ_LOGS(x) x

    #define ez_printf_null(...) {\
            ez_log_L(EZ_LOG_LEVEL_NONE, "%s(%d) ", __FILE__, __LINE__);\
            ez_log_cnt_L(EZ_LOG_LEVEL_NONE, __VA_ARGS__);\
        }
    #define ez_printf_debug(...)    {\
            ez_log_L(EZ_LOG_LEVEL_DEBUG, "%s(%d) ", __FILE__, __LINE__);\
            ez_log_cnt_L(EZ_LOG_LEVEL_DEBUG, __VA_ARGS__);\
        }
    #define ez_printf_info(...) {\
            ez_log_L(EZ_LOG_LEVEL_INFO, "%s(%d) ", __FILE__, __LINE__);\
            ez_log_cnt_L(EZ_LOG_LEVEL_INFO, __VA_ARGS__);\
        }
    #define ez_printf_info_content(...) {\
            ez_log_cnt_L(EZ_LOG_LEVEL_INFO, __VA_ARGS__);\
        }
    #define ez_printf_notice(...)   {\
            ez_log_L(EZ_LOG_LEVEL_NOTICE, "%s(%d) ", __FILE__, __LINE__);\
            ez_log_cnt_L(EZ_LOG_LEVEL_NOTICE, __VA_ARGS__);\
        }
    #define ez_printf_warning(...)  {\
            ez_log_L(EZ_LOG_LEVEL_WARNING, "%s(%d) ", __FILE__, __LINE__);\
            ez_log_cnt_L(EZ_LOG_LEVEL_WARNING, __VA_ARGS__);\
        }
    #define ez_printf_error(...)    {\
            ez_log_L(EZ_LOG_LEVEL_ERR, "%s(%d) ", __FILE__, __LINE__);\
            ez_log_cnt_L(EZ_LOG_LEVEL_ERR, __VA_ARGS__);\
        }
    #define ez_printf_crit(...) {\
            ez_log_L(EZ_LOG_LEVEL_CRIT, "%s(%d) ", __FILE__, __LINE__);\
            ez_log_cnt_L(EZ_LOG_LEVEL_CRIT, __VA_ARGS__);\
        }
    #define ez_printf_alert(...)    {\
            ez_log_L(EZ_LOG_LEVEL_ALERT, "%s(%d) ", __FILE__, __LINE__);\
            ez_log_cnt_L(EZ_LOG_LEVEL_ALERT, __VA_ARGS__);\
        }
    #define ez_printf_emerg(...)    {\
            ez_log_L(EZ_LOG_LEVEL_EMERG, "%s(%d) ", __FILE__, __LINE__);\
            ez_log_cnt_L(EZ_LOG_LEVEL_EMERG, __VA_ARGS__);\
        }

    #define ez_printf_file(...) ez_log(__VA_ARGS__)
#elif defined(_USE_EZLOG_FILENAME)
    #define EZ_LOGS(x) x

    #define ez_printf_null(...) {\
            ez_log_L(EZ_LOG_LEVEL_NONE, "%s(%d) ", __FILENAME__, __LINE__);\
            ez_log_cnt_L(EZ_LOG_LEVEL_NONE, __VA_ARGS__);\
        }
    #define ez_printf_debug(...)    {\
            ez_log_L(EZ_LOG_LEVEL_DEBUG, "%s(%d) ", __FILENAME__, __LINE__);\
            ez_log_cnt_L(EZ_LOG_LEVEL_DEBUG, __VA_ARGS__);\
        }
    #define ez_printf_info(...) {\
            ez_log_L(EZ_LOG_LEVEL_INFO, "%s(%d) ", __FILENAME__, __LINE__);\
            ez_log_cnt_L(EZ_LOG_LEVEL_INFO, __VA_ARGS__);\
        }
    #define ez_printf_info_content(...) {\
            ez_log_cnt_L(EZ_LOG_LEVEL_INFO, __VA_ARGS__);\
        }
    #define ez_printf_notice(...)   {\
            ez_log_L(EZ_LOG_LEVEL_NOTICE, "%s(%d) ", __FILENAME__, __LINE__);\
            ez_log_cnt_L(EZ_LOG_LEVEL_NOTICE, __VA_ARGS__);\
        }
    #define ez_printf_warning(...)  {\
            ez_log_L(EZ_LOG_LEVEL_WARNING, "%s(%d) ", __FILENAME__, __LINE__);\
            ez_log_cnt_L(EZ_LOG_LEVEL_WARNING, __VA_ARGS__);\
        }
    #define ez_printf_error(...)    {\
            ez_log_L(EZ_LOG_LEVEL_ERR, "%s(%d) ", __FILENAME__, __LINE__);\
            ez_log_cnt_L(EZ_LOG_LEVEL_ERR, __VA_ARGS__);\
        }
    #define ez_printf_crit(...) {\
            ez_log_L(EZ_LOG_LEVEL_CRIT, "%s(%d) ", __FILENAME__, __LINE__);\
            ez_log_cnt_L(EZ_LOG_LEVEL_CRIT, __VA_ARGS__);\
        }
    #define ez_printf_alert(...)    {\
            ez_log_L(EZ_LOG_LEVEL_ALERT, "%s(%d) ", __FILENAME__, __LINE__);\
            ez_log_cnt_L(EZ_LOG_LEVEL_ALERT, __VA_ARGS__);\
        }
    #define ez_printf_emerg(...)    {\
            ez_log_L(EZ_LOG_LEVEL_EMERG, "%s(%d) ", __FILENAME__, __LINE__);\
            ez_log_cnt_L(EZ_LOG_LEVEL_EMERG, __VA_ARGS__);\
        }

    #define ez_printf_file(...) ez_log(__VA_ARGS__)
#else//_USE_EZLOG _USE_EZLOG_CLASSIC
    #define EZ_LOGS(x)

    // do not use any more
    #define EZ_LOGS_INFO(x)
    // do not use any more
    #define EZ_LOGS_WARN(x)
    // do not use any more
    #define EZ_LOGS_ERROR(x)
    //
    #define ez_printf_null(...)
    #define ez_printf_debug(...)
    #define ez_printf_info(...)
    #define ez_printf_info_content(...)
    #define ez_printf_notice(...)
    #define ez_printf_warning(...)
    #define ez_printf_error(...)
    #define ez_printf_crit(...)
    #define ez_printf_alert(...)
    #define ez_printf_emerg(...)

    #define ez_printf_file(...)
#endif//_USE_EZLOG

#endif //_ez_log_H


/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/* USAGE

#include "ez_log.h"

int main(int argc,char * argv[])
{
    int i;
    ez_log_structure("/data/qosmon.log", 2, 2*1024*1024);

    for (i=0;i<1000000;i++)
    {
        ez_log("This is a Log %04d from FILE:%s LINE:%d\n", i, __FILE__, __LINE__);
    }

    ez_log_destructor();

    return 0;
}

// log with level
int test_ezlog2(int argc,char * argv[])
{
    int i;
    char *fname = argc>1?argv[1]:"abc.log";

    ez_log_structure(fname, 2, 1024*1024);
    printf("ez_log_structure:[%s]\n", fname);
    // int __llevel = EZ_LOG_TO_STDOUT*10+EZ_LOG_LEVEL_INFO;
    ez_log_set_level(17);
    ez_log_L(5, "%s(%d) -I- can see me at stdout\n", __FILE__, __LINE__);

    ez_log_set_level(14);
    ez_log_L(5, "%s(%d) -I- can not see me at stdout\n", __FILE__, __LINE__);

    ez_log_set_level(27);
    ez_log_L(5, "%s(%d) -I- can see me at file\n", __FILE__, __LINE__);

    ez_log_set_level(24);
    ez_log_L(5, "%s(%d) -I- can not see me at file\n", __FILE__, __LINE__);

    ez_log_set_level(37);
    ez_log_L(5, "%s(%d) -I- can see me at file and stdout\n", __FILE__, __LINE__);

    ez_log_set_level(34);
    ez_log_L(5, "%s(%d) -I- can not see me at file and stdout\n", __FILE__, __LINE__);

    ez_log_destructor();

    return 0;
}
*/
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
