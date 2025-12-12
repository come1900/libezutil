/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ez_c_printf.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ez_c_printf.h 5884 2021-10-24 10:38:12Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2021-10-24 10:38:12  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#if !defined(_ez_c_printf_H)
#define _ez_c_printf_H

#include <stdarg.h>
#ifdef __cplusplus

extern "C"
{
#endif

// Simple as we want
int ez_c_printf(const char * const, ...);

#ifdef __cplusplus
}
#endif


#endif // !defined(_ez_c_printf_H)
/*
RED = [r]
GREEN = [g]
YELLOW = [y]
BLUE = [b]
MAGENTA = [m]
CYAN = [c]
NORMAL = [n]

ez_c_printf("[r]%s", "red text");
ez_c_printf("[g]%s", "green text");
ez_c_printf("[r]%s [g]%s", "red", "green");
ez_c_printf("[y]%d", 52334);
ez_c_printf("[r]%s %s", "red", "normal");
*/