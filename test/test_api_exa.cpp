/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * test_api_exa.cpp - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: test_api_exa.cpp 5884 2012-10-24 10:56:45Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-10-24 10:56:45  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#include <stdio.h>
#include <stdlib.h>
#include "api_exa.h"
 
int main(int argc, char *argv[])
{
    int a = 1;
    int b=2;
    int c=0;

    c= a_plus_b(a, b);

    printf("a_plus_b(%d, %d):%d\n", a, b, c);
 
   return 0;
}
