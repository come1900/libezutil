/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * replace_chars_with_chars-msrc-nginx.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: replace_chars_with_chars-msrc-nginx.c 5884 2021-09-20 10:02:56Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2021-09-20 10:02:56  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ezutil/str_opr.h>
#include <ezutil/ez_fs.h>
#include <ezutil/ez_c_printf.h>

int main(int argc, char *argv[])
{

    //ngx_http_special_response.c
    do
    {
        char *pFilePathName = "./ngx_http_special_response.c";
        char *pBuffer;

        printf("process:%s ", pFilePathName);
        //ez_c_printf("process:[g]%s", pFilePathName);

        int iret = ez_load_file_to_memory(pFilePathName, &pBuffer);

        if (iret < 0)
        {
            ez_c_printf("ez_load_file_to_memory(%d):[r]%s\n", iret, "failed");

            break;
        }

        if (0==iret)
        {
            //ez_c_printf("ez_load_file_to_memory failed:[r]%d\n", iret);
            ez_c_printf("ez_load_file_to_memory(%d):[r]%s\n", iret, "failed");

            break;
        }

        char *p_ret = replace_chars_with_chars(pBuffer, "<hr><center>nginx</center>\" CRL", "<hr><center>pssix</center>\" CRL");
        if (NULL == p_ret )
        {
            //printf("replace_chars_with_chars failed!\n");
            ez_c_printf("replace_chars_with_chars [r]%s\n", "failed");

            break;
        }

        iret = ez_save_file_from_memory(pFilePathName, pBuffer, strlen(pBuffer));

        if (iret < 0)
        {
            //printf("ez_save_file_from_memory failed(%d):%s\n", iret, pFilePathName);
            ez_c_printf("ez_save_file_from_memory(%d):[r]%s\n", iret, "failed");

            break;
        }

        if (pBuffer)
        {
            free(pBuffer);
            pBuffer = NULL;
        }
        
        ez_c_printf("[g]%s.\n", "success");
    }
    while(0);

    //ngx_setproctitle.c
    do
    {
        char *pFilePathName = "./ngx_setproctitle.c";
        char *pBuffer;
        printf("process:%s ", pFilePathName);
        int iret = ez_load_file_to_memory(pFilePathName, &pBuffer);

        if (iret < 0)
        {
            printf("ez_load_file_to_memory failed(%d):%s\n", iret, pFilePathName);

            break;
        }

        if (0==iret)
        {
            printf("ez_load_file_to_memory failed(%d):%s\n", iret, pFilePathName);

            break;
        }

        char *p_ret = replace_chars_with_chars(pBuffer, "p = ngx_cpystrn((u_char *) ngx_os_argv[0], (u_char *) \"nginx: \",", "p = ngx_cpystrn((u_char *) ngx_os_argv[0], (u_char *) \"pssix: \",");
        if (NULL == p_ret )
        {
            printf("replace_chars_with_chars failed!\n");

            break;
        }

        iret = ez_save_file_from_memory(pFilePathName, pBuffer, strlen(pBuffer));

        if (iret < 0)
        {
            printf("ez_save_file_from_memory failed(%d):%s\n", iret, pFilePathName);

            break;
        }

        if (pBuffer)
        {
            free(pBuffer);
            pBuffer = NULL;
        }

        ez_c_printf("[g]%s.\n", "success");

    }
    while(0);

    //nginx.h
    do
    {
        char *pFilePathName = "./nginx.h";
        char *pBuffer;
        printf("process:%s ", pFilePathName);

        int iret = ez_load_file_to_memory(pFilePathName, &pBuffer);

        if (iret < 0)
        {
            printf("ez_load_file_to_memory failed(%d):%s\n", iret, pFilePathName);

            break;
        }

        if (0==iret)
        {
            printf("ez_load_file_to_memory failed(%d):%s\n", iret, pFilePathName);

            break;
        }

        char *p_ret = replace_chars_with_chars(pBuffer, "\"NGINX\"", "\"PSSIX\"");
        if (NULL == p_ret )
        {
            printf("replace_chars_with_chars failed!\n");

            break;
        }

        p_ret = replace_chars_with_chars(pBuffer, "\"nginx/\" NGINX_VERSION", "\"pssix/\" NGINX_VERSION");
        if (NULL == p_ret )
        {
            printf("replace_chars_with_chars failed!\n");

            break;
        }

        iret = ez_save_file_from_memory(pFilePathName, pBuffer, strlen(pBuffer));

        if (iret < 0)
        {
            printf("ez_save_file_from_memory failed(%d):%s\n", iret, pFilePathName);

            break;
        }

        if (pBuffer)
        {
            free(pBuffer);
            pBuffer = NULL;
        }
        ez_c_printf("[g]%s.\n", "success");

    }
    while(0);

    //nginx.c
    do
    {
        char *pFilePathName = "./nginx.c";
        char *pBuffer;

        printf("process:%s ", pFilePathName);

        int iret = ez_load_file_to_memory(pFilePathName, &pBuffer);

        if (iret < 0)
        {
            printf("ez_load_file_to_memory failed(%d):%s\n", iret, pFilePathName);

            break;
        }

        if (0==iret)
        {
            printf("ez_load_file_to_memory failed(%d):%s\n", iret, pFilePathName);

            break;
        }

        char *p_ret = replace_chars_with_chars(pBuffer, "ngx_write_stderr(\"nginx version", "ngx_write_stderr(\"pssix version");
        if (NULL == p_ret )
        {
            printf("replace_chars_with_chars failed:%s\n", "ngx_write_stderr(\"nginx version");

            break;
        }

        p_ret = replace_chars_with_chars(pBuffer, "\"Usage: nginx [-", "\"Usage: pssix [-");
        if (NULL == p_ret )
        {
            printf("replace_chars_with_chars failed:%s\n", "\"Usage: nginx [-");

            break;
        }

        iret = ez_save_file_from_memory(pFilePathName, pBuffer, strlen(pBuffer));

        if (iret < 0)
        {
            printf("ez_save_file_from_memory failed(%d):%s\n", iret, pFilePathName);

            break;
        }

        if (pBuffer)
        {
            free(pBuffer);
            pBuffer = NULL;
        }
        ez_c_printf("[g]%s.\n", "success");

    }
    while(0);
    
    return 0;
}
