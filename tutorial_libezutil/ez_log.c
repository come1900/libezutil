/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ez_fs.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ez_fs.c 5884 2012-09-29 02:48:19Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-09-29 02:48:19  Create
 *     2015-06-24 10:35:35  Mod
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define _USE_EZLOG 1
#define _USE_EZLOG_CLASSIC 1
#include <ezutil/ez_log.h>

int main(int argc,char * argv[])
{
    int i;
    char *fname = argc>1?argv[1]:"test_log.log";
    int iLevel = EZ_LOG_TO_FILE*10+EZ_LOG_LEVEL_DEBUG;
    	
    ez_log_structure(fname, 3, 3*1024*1024);
    ez_log_set_level(iLevel);

    printf("ezlog_structure:[%s]\n", fname);

    for (i=0;i<250000/3/*zlog*/;i++)
    {
//		ez_printf_error("%s(%d) i:%-8d\n", __FILE__, __LINE__, i);
//		ez_printf_info("%s(%d) i:%-8d\n", __FILE__, __LINE__, i);
//		ez_printf_debug("%s(%d) i:%-8d\n", __FILE__, __LINE__, i);
		ez_printf_error("i:%-8d\n", i);
		ez_printf_info("i:%-8d\n", i);
		ez_printf_debug("i:%-8d\n", i);
    }

    ez_log_destructor();

    return 0;
}
