/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * http_query_parser.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: http_query_parser.c 5884 2019-08-15 11:17:59Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2019-08-15 11:17:59  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ez_http_query_parser.h"

int main(int argc, char *argv[])
{
    const char *__pQuery = "app=live&flashver=FMLE/3.0%20(compatible%3B%20Lavf58.20&swfurl=&tcurl=rtmp://127.0.0.1:19090/live&pageurl=&addr=127.0.0.1&clientid=5&call=publish&name=8128CD2009E_0_1&type=live";

    char strApp[100] = "";
    char strName[100] = "";

    ez_getParam_HttpQuery(__pQuery, "app", strApp);
    ez_getParam_HttpQuery(__pQuery, "name", strName);

    printf("%16s:[%s]\n", "app",  strApp);
    printf("%16s:[%s]\n", "name", strName);

    ez_getParam_HttpQuery(__pQuery, "noThisPara", strName);
    printf("%16s:[%s]\n", "noThisPara", strName);

    return 0;
}
