/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * test_ezutil.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: test_ezutil.c 5884 2014-08-01 04:32:28Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2014-08-01 04:32:28  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <../src/ez_log.h>
int test_ezlog(int argc,char * argv[]);
int test_ezlog2(int argc,char * argv[]);

#include "../src/ez_fs.h"
#include "../src/md5.h"
int test_file_sniff(int argc,char * argv[]);

int main(int argc,char * argv[])
{
    int iret;

	iret = test_ezlog2(argc, argv);
	//iret = test_file_sniff(argc, argv);

    return 0;
}

int test_ezlog(int argc,char * argv[])
{
    int i;
    char *fname = argc>1?argv[1]:"abc.log";

    ez_log_structure(fname, /*"def.log"*/2, 1024*1024);
    printf("ezlog_structure:[%s]\n", fname);

    for (i=0;i<1000000;i++)
    {
        ez_log("This is a Log %04d from FILE:%s LINE:%d\n", i, __FILE__, __LINE__);
    }

    ez_log_destructor();
	
	return 0;
}

int test_ezlog2(int argc,char * argv[])
{
    //int i;
    char *fname = argc>1?argv[1]:"abc.log";

    ez_log_structure(fname, /*"def.log"*/2, 1024*1024);
    printf("ezlog_structure:[%s]\n", fname);

	ez_log_set_level(17);
	ez_log_L(5, "%s(%d) -I- can see me at stdout\n", __FILE__, __LINE__);
	ez_printf_info("%s(%d) -I- can see me at stdout\n", __FILE__, __LINE__);

	ez_log_set_level(14);
	ez_log_L(5, "%s(%d) -I- can not see me at stdout\n", __FILE__, __LINE__);

	ez_log_set_level(27);
	ez_log_L(5, "%s(%d) -I- can see me at file\n", __FILE__, __LINE__);

	ez_log_set_level(24);
	ez_log_L(5, "%s(%d) -I- can not see me at file\n", __FILE__, __LINE__);

	ez_log_set_level(37);
	//ez_log_L(5, "%s(%d) -I- can see me at file and stdout\n", __FILE__, __LINE__);
	ez_printf_info("%s(%d) -I- can see me at stdout\n", __FILE__, __LINE__);


	ez_log_set_level(34);
	ez_log_L(5, "%s(%d) -I- can not see me at file and stdout\n", __FILE__, __LINE__);

	ez_log_destructor();
	
	return 0;
}

int test_file_sniff(int argc,char * argv[])
{
	int iFileLenOri = 0;
	int iFileLenNew = 0;
	int signedStrLen = 0;
	
	char *pBufff;
	char *pTmpBufer = NULL;

	//iFileLenOri = ez_load_file_to_memory("D:\\tmp\\a.jpg", &pBufff);
	iFileLenOri = ez_load_file_to_memory("test_libezutil.vcxproj", &pBufff);
	printf("ez_load_file_to_memory:%d\n", iFileLenOri);

//	iFileLenNew = ez_save_file_from_memory("b.jpg", pBufff, iFileLenOri);
//	printf("ez_save_file_from_memory:%d\n", iFileLenNew);
//	return 0;

	if (pBufff && iFileLenOri>0)
	{
#if 1
		char vString[128]={0};

		ez_md5_calc_hexstr(vString, pBufff, iFileLenOri);
		signedStrLen = strlen(vString);
		//pTmpBufer[signedStrLen]
		printf("verify code[%d]:%s\n", signedStrLen, vString);
		// 

		iFileLenNew = ez_save_file_from_memory_memory("b.jpg", pBufff, iFileLenOri, vString, signedStrLen);

		printf("iFileLenOri:%d, iFileLenNew:%d\n", iFileLenOri, iFileLenNew);

#endif
		free(pBufff);
	}

	return 0;
}
