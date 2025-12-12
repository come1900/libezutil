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
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ezutil/ez_fs.h>

const char * const pExistFile = __FILE__;
const char * const pNotExistFile = __FILE__".NotExist";

int main(int argc, char *argv[])
{
    char *content = NULL;
    int size;

    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

    content = NULL;
    size = ez_load_file_to_memory(pExistFile, &content);

    if (size < 0)
    {
        printf("Error loading file:%s\n", pExistFile);
    }
    else
    {
        printf("file size:%d\n", size);
    }

    if (content != NULL)
    {
        free (content);
        content = NULL;
    }
    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

    content = NULL;
    size = ez_load_file_to_memory(pNotExistFile, &content);

    if (size < 0)
    {
        printf("Error loading file:%s\n", pNotExistFile);
    }
    else
    {
        printf("file size:%d\n", size);
    }

    if (content != NULL)
    {
        free (content);
        content = NULL;
    }
    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

    return 0;
}
