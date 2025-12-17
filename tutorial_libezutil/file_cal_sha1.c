/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * file_cal_sha1.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: file_cal_sha1.c 5884 2015-09-30 10:21:16Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2015-09-30 10:21:16  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <ezutil/sha1.h>

#ifndef lint
static char copyright[] =
    "@(#) Copyright (c) 2015\n\
    wujj@gminitech.com  All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char rcsid[] = "$Tools: file_cal_sha1.c,v 1.0 2015-09-30 10:20 joy.woo Exp $";
#endif /* not lint */

const int FILE_PICE_SIZE = 4096;

int file_sha1(char *output, char *p_filename);

/*
 * Copy from from to to.  Intended for use in system installation.
 */
int main(int argc, char *argv[])
{
    //extern int errno;
    //register int from, to, record, rcc, wcc, bsize = BSIZE;
    //char buf[BSIZE];

    int iret;

    if (argc < 2)
    {
#ifndef lint
        printf("%s %s\n", copyright, rcsid);
#endif

        printf("%s <source>\n", argv[0]);

        return -1;
    }

    char md5str[128+1]={0};
    iret = file_sha1(md5str, argv[1]);
    if (iret >= 0)
    {
        printf("%s  %s\n", md5str, argv[1]);
    }
    else
    {
        printf("%s %s\n", "file_sha1 failed:", argv[1]);
    }

    return 0;
}

int file_sha1(char *output, char *p_filename)
{
    assert(output);
    assert(p_filename);

    int ret=0;
    char buf[FILE_PICE_SIZE];
    int from, rcc, err;

    from = open(p_filename, O_RDONLY);
if (from<=0)
{
fprintf(stderr, "open failed:%d.\n", errno);
}

    //__MD5_CTX context;
    //ez_MD5Init(&context);

    SHA1Context sha;
    err = SHA1Reset(&sha);
    if (err)
    {
        fprintf(stderr, "SHA1Reset Error %d.\n", err );
        return -1;
    }

    do
    {
        rcc = read(from, buf, sizeof(buf));

        if (rcc>0)
        {
            //ez_MD5Update(&context, (unsigned char *)buf, rcc);
            err = SHA1Input(&sha,
                            (const unsigned char *) buf,
                            rcc);
            if (err)
            {
                fprintf(stderr, "SHA1Input Error %d.\n", err );
                break;    /* out of for i loop */
            }
        }
        else if (rcc == 0)
        {
            // read finished cal it
            char szTemp[4] = {0};
            output[0]='\0'; // set string len to 0
            unsigned char outputtemp[20]={0};
            //ez_MD5Final(outputtemp, &context);
            err = SHA1Result(&sha, outputtemp);

            //printf("MD5:");
            int iPrint=0;
            for (iPrint = 0; iPrint < 20; iPrint++)
            {
                sprintf(szTemp,"%02x",outputtemp[iPrint]);
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
