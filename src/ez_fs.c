/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ez_fs.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ez_fs.c 5884 2012-09-29 02:21:25Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-09-29 02:21:25  Create
 *     2014-06-20 10:24:55  Modify, support /home/usr/sbin/logs dir
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#else//if _LINUX
#include <stdarg.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#include "ez_fs.h"

#if defined(_WIN32)
#define EZ_ACCESS _access
#define EZ_MKDIR(a) _mkdir((a))
#ifndef F_OK
#define F_OK 0
#endif
#else//if defined(_LINUX)
#define EZ_ACCESS access
#define EZ_MKDIR(a) mkdir((a),0755)
#endif


#ifndef _DEBUG_THIS
//  #define _DEBUG_THIS
#endif
#ifdef _DEBUG_THIS
	#define DEB(x) x
	#define DBG(x) x
#else
	#define DEB(x)
	#define DBG(x)
#endif

#ifndef __trip
	#define __trip printf("-W-%d::%s(%d)\n", (int)time(NULL), __FILE__, __LINE__);
#endif
#ifndef __fline
	#define __fline printf("%s(%d)--", __FILE__, __LINE__);
#endif

#define ARG_USED(x) (void)&x;


int ez_creat_dir(const char *const pdir)
{
    int i = 0;
    int iRet;
    int iLen;
    char* pszDir;

    if(NULL == pdir)
    {
        return -1;
    }

    pszDir = strdup(pdir);
    if (NULL == pszDir)
    {
        return -2;
    }
    iLen = strlen(pszDir);
    DBG(
        printf("pdir:[%s], iRet:%d\n", pdir, iLen);
    );

    // 创建中间目录
    for (i = 0;i < iLen;i ++)
    {
        if (pszDir[i] == '\\' || pszDir[i] == '/')
        {
            // 2014-06-20 10:25:29
            //这种格式：/home/usr/sbin/logs， 造成第一个pszDir 为空串 引起下面的失败返回-3
            //
            if (i==0)
            {
                continue;
            }

            pszDir[i] = '\0';

            //如果不存在,创建
            iRet = EZ_ACCESS(pszDir, F_OK);
            DBG(
                printf("EZ_ACCESS:[%s], iRet:%d\n", pszDir, iRet);
            );
            if (iRet != 0)
            {
                iRet = EZ_MKDIR(pszDir);
                DBG(
                    printf("EZ_MKDIR:[%s], iRet:%d\n", pszDir, iRet);
                );
                if (iRet != 0)
                {
                    if(pszDir)
                    {
                        free(pszDir);
                    }
                    return -3;
                }
            }
            //支持linux,将所有\换成/
            pszDir[i] = '/';
        }
    }

    iRet = EZ_MKDIR(pszDir);
    if (iRet < 0)
    {
        if (EEXIST == errno)
        {
            iRet = 100;
        }
    }

    if(pszDir)
    {
        free(pszDir);
    }
    return iRet;
}

int ez_check_file_exist(const char *const pfilename)
{
    int iret = 0;
            //如果不存在,创建
    iret = EZ_ACCESS(pfilename, F_OK);
    DBG(
        printf("EZ_ACCESS:[%s], iRet:%d\n", pszDir, iRet);
    );

    return (iret == 0);

    //return iret;
}


int ez_load_file_to_memory(const char *filename, char **result)
{
    int size = 0;
    FILE *f = fopen(filename, "rb");
    if (f == NULL)
    {
        *result = NULL;
        return -1; // -1 means file opening fail
    }

    fseek(f, 0, SEEK_END);
    size = ftell(f);
    if (size<1)
    {
        fclose(f);
        return 0;
    }

    fseek(f, 0, SEEK_SET);
    *result = (char *)malloc(size+1);

    if (*result==NULL)
    {
        fclose(f);
        return -2;
    }

    if (size != fread(*result, sizeof(char), size, f))
    {
        free(*result);
        *result=NULL;
        fclose(f);
        return -3; // -2 means file reading fail
    }

    fclose(f);
    (*result)[size] = '\0';

    return size;
}

int ez_save_file_from_memory(const char *filename, const char *buff, const int buff_len)
{
	int size = 0;
	FILE *f = NULL;

	if (filename==NULL || buff==NULL || buff_len<1)
	{
		return -1;
	}

	f = fopen(filename, "w+");
	if (f == NULL)
	{
		//*result = NULL;
		return -2; // -1 means file opening fail
	}
	//fseek(f, 0, SEEK_END);
	//size = ftell(f);
	//fseek(f, 0, SEEK_SET);
	//*result = (char *)malloc(size+1);
	size = fwrite(buff, sizeof(char), buff_len, f);
	if (size != buff_len)
	{
		//free(*result);
		//return -3; // -2 means file writing fail
		size = -3;
	}
	fclose(f);
	//(*result)[size] = 0;
	return size;
}

int ez_append_file_from_memory(const char *filename, const char *buff, const int buff_len)
{
	int size = 0;
	FILE *f = NULL;

	if (filename==NULL || buff==NULL || buff_len<1)
	{
		return -1;
	}

	f = fopen(filename, "a+");
	if (f == NULL)
	{
		//*result = NULL;
		return -2; // -1 means file opening fail
	}
	//fseek(f, 0, SEEK_END);
	//size = ftell(f);
	//fseek(f, 0, SEEK_SET);
	//*result = (char *)malloc(size+1);
	size = fwrite(buff, sizeof(char), buff_len, f);
	if (size != buff_len)
	{
		//free(*result);
		//return -3; // -2 means file writing fail
		size = -3;
	}
	fclose(f);
	//(*result)[size] = 0;
	return size;
}
int ez_save_file_from_memory_memory(
			const char *filename
			, const char *buff
			, const int buff_len
			, const char *buff2
			, const int buff2_len)
{
	int size = 0;
	FILE *f = NULL;
	int iret = 0;

	if (filename==NULL || buff==NULL || buff_len<1)
	{
		return -1;
	}

	f = fopen(filename, "w+");
	if (f == NULL)
	{
		//*result = NULL;
		return -2; // -1 means file opening fail
	}
	//fseek(f, 0, SEEK_END);
	//size = ftell(f);
	//fseek(f, 0, SEEK_SET);
	//*result = (char *)malloc(size+1);

	// write buf 1;
	size = fwrite(buff, sizeof(char), buff_len, f);
	if (size != buff_len)
	{
		//free(*result);
		//return -3; // -2 means file writing fail
		iret = -3;
	}
	else
	{
		size = fwrite(buff2, sizeof(char), buff2_len, f);
		if (size != buff2_len)
		{
			//free(*result);
			//return -3; // -2 means file writing fail
			iret = -4;
		}
		else
		{
			iret = buff_len + buff2_len;
		}
		
	}
	
	fclose(f);
	//(*result)[size] = 0;
	return iret;
}
