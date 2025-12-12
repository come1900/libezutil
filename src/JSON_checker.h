/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * JSON_checker.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: JSON_checker.h 5884 2020-05-21 05:37:57Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2020-05-21 05:37:57  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#if !defined(_JSON_checker_H)
#define _JSON_checker_H

#ifdef __cplusplus
extern "C"
{
#endif

//str_len: 0- willbe strlen ...
extern int json_checker(const char *json_str, const size_t str_len);

#ifdef __cplusplus
}
#endif

#endif // !defined(_JSON_checker_H)
