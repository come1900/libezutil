/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ez_color_printf.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ez_color_printf.c 5884 2021-10-24 10:34:39Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2021-10-24 10:34:39  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ezutil/ez_c_printf.h>

int main(int argc,char * argv[])
{
    ez_c_printf("[r]%s ", "red text");
    ez_c_printf("[g]%s ", "green text");
    ez_c_printf("[r]%s [g]%s ", "red", "green");
    ez_c_printf("[y]%d ", 52334);
    ez_c_printf("[r]%s %s ", "red", "normal");

    printf("\n");

    return 0;
}
