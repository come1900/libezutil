/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * simple_cfg_parse.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: simple_cfg_parse.c 5884 2015-11-10 02:12:04Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2015-11-10 02:12:04  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

/*
support:
name=joy
  sex=male
age 100
add:galaxy
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ezutil/ez_fs.h>
#include <ezutil/str_opr.h>

int main(int argc, char *argv[])
{
	char *p_file_name = __FILE__;
	char *pFileBuffer;
	char str_value_buf[1024] = {'\0'};
	int iFileLen = 0;

	iFileLen = ez_load_file_to_memory(p_file_name, &pFileBuffer);

	if (iFileLen<=0)
	{
		printf("ez_load_file_to_memory failed\n");

		return -1;
	}

	{
		int iret = 0;
		char *pFieldName = "name";
		iret = ez_get_ext_value(pFileBuffer, iFileLen, pFieldName, str_value_buf, sizeof(str_value_buf));
		printf("FieldName:%8s :(%2d) %s\n", pFieldName, iret, str_value_buf);
	}
	{
		int iret = 0;
		char *pFieldName = "sex";
		iret = ez_get_ext_value(pFileBuffer, iFileLen, pFieldName, str_value_buf, sizeof(str_value_buf));
		printf("FieldName:%8s :(%2d) %s\n", pFieldName, iret, str_value_buf);
	}
	{
		int iret = 0;
		char *pFieldName = "age";
		iret = ez_get_ext_value(pFileBuffer, iFileLen, pFieldName, str_value_buf, sizeof(str_value_buf));
		printf("FieldName:%8s :(%2d) %s\n", pFieldName, iret, str_value_buf);
	}
	{
		int iret = 0;
		char *pFieldName = "add";
		iret = ez_get_ext_value(pFileBuffer, iFileLen, pFieldName, str_value_buf, sizeof(str_value_buf));
		printf("FieldName:%8s :(%2d) %s\n", pFieldName, iret, str_value_buf);
	}

	if (pFileBuffer)
	{
		free(pFileBuffer);
	}
    
    return 0;
}
