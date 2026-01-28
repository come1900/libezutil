/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * file_cal_md5.c - /home/wujj/src/libs/src_ezutil
 *
 * Copyright (C) 2006 ezlibs.com, All Rights Reserved.
 *
 * $Id: file_cal_md5.c, v 1.0.0.1 2006-03-11 13:23:35 wjj Exp $
 *
 * _detail_explain_.
 * 
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <ezutil/md5.h>

#ifndef lint
static char copyright[] =
    "@(#) Copyright (c) 2006\n\
    wujj@gminitech.com  All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char rcsid[] = "$Tools: file_cal_md5.c,v 1.0 2006-3-11 13:22 joy.woo Exp $";
#endif /* not lint */

const int const FILE_PICE_SIZE = 4096;

int file_md5(char *output, char *p_filename);

/*
 * Copy from from to to.  Intended for use in system installation.
 */
int main(int argc, char *argv[])
{
    //extern int errno;
    //register int from, to, record, rcc, wcc, bsize = BSIZE;
    //char buf[BSIZE];

    if (argc < 2)
    {
#ifndef lint
        printf("%s %s\n", copyright, rcsid);
#endif

        printf("%s <source>\n", argv[0]);

        return -1;
    }

    char md5str[FLIE_MD5_HASH_STR_LEN+1]={0};
    file_md5(md5str, argv[1]);
    printf("md5str:%s\n", md5str);

    ez_file_md5(md5str, argv[1]);
    printf("md5str:%s\n", md5str);
    
    return 0;
}

int file_md5(char *output, char *p_filename)
{
    assert(output);
    assert(p_filename);

    int ret=0;
    char buf[FILE_PICE_SIZE];
    int from, rcc;

    from = open(p_filename, O_RDONLY);

    __MD5_CTX context;
    ez_MD5Init(&context);

    do
    {
        rcc = read(from, buf, sizeof(buf));

        if (rcc>0)
        {
            ez_MD5Update(&context, (unsigned char *)buf, rcc);
        }
        else if (rcc == 0)
        {
            // read finished cal it
            char szTemp[4] = {0};
            output[0]='\0'; // set string len to 0
            unsigned char outputtemp[16]={0};
            ez_MD5Final(outputtemp, &context);

            //printf("MD5:");
            int iPrint=0;
            for (iPrint = 0; iPrint < 16; iPrint++)
            {
                sprintf(szTemp,"%02X",outputtemp[iPrint]);
                strcat((char *)output, szTemp);
            }
            ret = strlen(output);

            break;
        }
        else// if (rcc < 0)
        {
            ret = -100;
            printf("read error, errno=%d\n", errno);
            break;
        }
    }
    while(rcc>0);

    close(from);

    return ret;
}
