/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 *  * ttst.c - _explain_
 *   *
 *    * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *     *
 *      * $Id: ttst.c 5884 2016-11-23 02:28:14Z WuJunjie $
 *       *
 *        *  Explain:
 *         *     -explain-
 *          *
 *           *  Update:
 *            *     2016-11-23 02:28:14  Create
 *             */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <stdio.h>
#include <unistd.h>
//#include <io.h>

int main(int argc, char *argv[])
{
    if (argc<2)
    {
        printf("%s <filePathName>\n", argv[0]);
        return -1;
    }

    if ( !access(argv[1],0) )
    {
        printf("%s exist\n", argv[1]);
    }
    else
    {
        printf("%s Not exist\n", argv[1]);
    }
    return 0;
}

