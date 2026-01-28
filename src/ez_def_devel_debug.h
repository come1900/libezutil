/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ez_def_devel_debug.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ez_def_devel_debug.h 5884 2020-04-15 02:39:58Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2020-04-15 02:39:58  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#if !defined(_ez_def_devel_debug_H)
#define _ez_def_devel_debug_H

#ifndef _DEBUG_THIS
//#define _DEBUG_THIS
#endif
#ifdef _DEBUG_THIS
    #define DEB(x) x
    #define DBG(x) x
#else
    #define DEB(x)
    #define DBG(x)
#endif

#ifndef __trip
	#define __trip printf("-W-%d::%s(%d)\n", (int)time(NULL), __FILE__, __LINE__);
#endif
#ifndef __fline
	#define __fline printf("%s(%d)--", __FILE__, __LINE__);
#endif

#define ARG_USED(x) (void)&x;


#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EZ_PRINT_LOG_LEVEL_DEBUG
#define EZ_PRINT_LOG_LEVEL_DEBUG 0
#endif

#ifndef EZ_PRINT_LOG_LEVEL_INFO
#define EZ_PRINT_LOG_LEVEL_INFO  1
#endif

#ifndef EZ_PRINT_LOG_LEVEL_WARN
#define EZ_PRINT_LOG_LEVEL_WARN  2
#endif

#ifndef EZ_PRINT_LOG_LEVEL_ERROR
#define EZ_PRINT_LOG_LEVEL_ERROR 3
#endif

#ifndef EZ_PRINT_LOG_LEVEL
#define EZ_PRINT_LOG_LEVEL EZ_PRINT_LOG_LEVEL_INFO
#endif

static inline void ez_get_timestamp(char *buf, size_t buf_size)
{
	struct timespec ts;
	struct tm *tm_info;
	clock_gettime(CLOCK_REALTIME, &ts);
	tm_info = localtime(&ts.tv_sec);
	snprintf(buf, buf_size, "%02d:%02d:%02d.%03ld",
	         tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec,
	         ts.tv_nsec / 1000000);
}

static inline const char *ez_extract_filename(const char *path)
{
	const char *filename = strrchr(path, '/');
	return filename ? filename + 1 : path;
}

#ifndef EZ_PRINT_LOG_PRINT
#define EZ_PRINT_LOG_PRINT(level_str, ...)                                              \
	do {                                                                       \
		char ts[32];                                                           \
		ez_get_timestamp(ts, sizeof(ts));                                      \
		printf("[%s] [%s] [%s:%d] ", ts, level_str,                            \
		       ez_extract_filename(__FILE__), __LINE__);                       \
		printf(__VA_ARGS__);                                                   \
	} while (0)
#endif

#if EZ_PRINT_LOG_LEVEL <= EZ_PRINT_LOG_LEVEL_DEBUG
#define EZ_PRINT_LOG_DEBUG(...) EZ_PRINT_LOG_PRINT("DEBUG", __VA_ARGS__)
#else
#define EZ_PRINT_LOG_DEBUG(...)
#endif

#if EZ_PRINT_LOG_LEVEL <= EZ_PRINT_LOG_LEVEL_INFO
#define EZ_PRINT_LOG_INFO(...) EZ_PRINT_LOG_PRINT("INFO", __VA_ARGS__)
#else
#define EZ_PRINT_LOG_INFO(...)
#endif

#if EZ_PRINT_LOG_LEVEL <= EZ_PRINT_LOG_LEVEL_WARN
#define EZ_PRINT_LOG_WARN(...) EZ_PRINT_LOG_PRINT("WARN", __VA_ARGS__)
#else
#define EZ_PRINT_LOG_WARN(...)
#endif

#if EZ_PRINT_LOG_LEVEL <= EZ_PRINT_LOG_LEVEL_ERROR
#define EZ_PRINT_LOG_ERROR(...) EZ_PRINT_LOG_PRINT("ERROR", __VA_ARGS__)
#else
#define EZ_PRINT_LOG_ERROR(...)
#endif

#ifndef EZ_PRINT_LOGF
#define EZ_PRINT_LOGF(...) EZ_PRINT_LOG_INFO(__VA_ARGS__)
#endif

#ifndef EZ_PRINT_DATA_LOGF
#define EZ_PRINT_DATA_LOGF(...) EZ_PRINT_LOG_INFO(__VA_ARGS__)
#endif

#ifdef __cplusplus
}
#endif

#endif // !defined(_ez_def_devel_debug_H)a

