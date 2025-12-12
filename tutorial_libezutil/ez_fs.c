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

int main(int argc, char *argv[])
{
    int iret;
    int ii;

    // test ez_creat_dir
    if (0)
    {
        //char *pDir = "c:\./tmplogs/1/2/3/4/5";
        char *pDir = ".\\tmplogs\\1/2/3/4/5";
        iret = ez_creat_dir(pDir);

        printf("CreatDir(%s):%d\n", pDir, iret);
    }

    for(ii=0; ii<1; ii++)
    {
        printf("(%-8d) -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n", ii);

        // test ez_save_file_from_memory
        if (1)
        {
            char *pFilePathName = "./ez_fs.memf";
            char *pBuffer="marry has a little lamp";
            iret = ez_save_file_from_memory(pFilePathName, pBuffer, strlen(pBuffer));

            if (iret>0)
            {
                printf("ez_save_file_from_memory(%d):\n[%s]\n", iret, pBuffer);
            }
            else
            {
                printf("ez_save_file_from_memory failed:%d\n", iret);
            }
        }

        // test ez_load_file_to_memory
        if (1)
        {
            char *pFilePathName = "./ez_fs.memf";
            char *pBuffer;
            iret = ez_load_file_to_memory(pFilePathName, &pBuffer);

            if (iret>0)
            {
                printf("ez_load_file_to_memory(%d):\n[%s]\n", iret, pBuffer);
            }
            else
            {
                printf("ez_load_file_to_memory failed:%d\n", iret);
            }

            if (pBuffer)
            {
                free(pBuffer);
                pBuffer = NULL;
            }
        }

        // test ez_load_file_to_memory
        if (1)
        {
            char *pFilePathName = __FILE__;
            iret = ez_check_file_exist(pFilePathName);

            printf("ez_check_file_exist(%s):%d\n", pFilePathName, iret);

            pFilePathName = "hehe__"__FILE__;
            iret = ez_check_file_exist(pFilePathName);
            printf("ez_check_file_exist(%s):%d\n", pFilePathName, iret);
        }

    }// for

    return 0;
}
