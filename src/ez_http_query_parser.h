/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ez_http_query_parser.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ez_http_query_parser.h 5884 2019-08-15 11:07:05Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2019-08-15 11:07:05  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


#ifndef _ez_http_query_parser_H
#define _ez_http_query_parser_H

#ifdef __cplusplus
extern "C" {
#endif

    /*
     * Declaration of function prototypes
     */
    int ez_getParam_HttpQuery(const char *InputData, const char *Name, char *Value);
;

#ifdef __cplusplus
}
#endif

#endif /* _ez_http_query_parser_H */
