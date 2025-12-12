/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ez_fs.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ez_fs.h 5884 2012-09-29 02:17:31Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-09-29 02:17:31  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


#ifndef _ez_fs_H
#define _ez_fs_H

#ifdef __cplusplus
extern "C"
{
#endif

	//use as
	//char *pDir = "c:\./tmplogs/1/2/3/4/5";
	//char *pDir = ".\\tmplogs\\1/2/3/4/5";
	int ez_creat_dir(const char *const pdir);
	// return 1- exist, 0-not exist
	int ez_check_file_exist(const char *const pfilename);
	
	//
	// result will be malloc size+1
	// return >=0 is filesize
	//    -1 open file failed
	//    -2 malloc failed
	//    -3 read file failed
	int ez_load_file_to_memory(const char *filename, char **result);
	// return filesize
	int ez_save_file_from_memory(const char *filename, const char *buff, const int buff_len);
	int ez_append_file_from_memory(const char *filename, const char *buff, const int buff_len);
	// return filesize(len+len2)
	int ez_save_file_from_memory_memory(const char *filename
			, const char *buff
			, const int buff_len
			, const char *buff2
			, const int buff2_len);


#ifdef __cplusplus
}
#endif

#endif //_ez_fs_H


/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/* USAGE
	//--------------------------
	printf("orig:%s\n", pOrig);
	usret = ez_crc16(0x0, (unsigned char *)pOrig, strlen(pOrig));
	printf("ez_crc16:0x%0X\n", usret);

	//--------------------------
	printf("orig:%s\n", pOrig);
	uiret = ez_crc32((unsigned char *)pOrig, strlen(pOrig));
	printf("ez_crc32:0x%0X\n", uiret);
	printf("this crc32 alo is:%s\n", uiret==0x5CFCCFF6?"OK":"NOK"); 
*/
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
