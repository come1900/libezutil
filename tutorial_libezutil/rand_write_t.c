/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * rand_write_t.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: rand_write_t.c 5884 2013-04-11 02:46:08Z WuJunjie $
 *
 *  Explain:
 *     -
 *          run as ./rand_write-linux-am335x 1-a.rand 1-b.rand > 1.log &
 *     -
 *
 *  Update:
 *     2013-04-08 02:46:08  Create
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
    "@(#) Copyright (c) 2013\n\
    ezlibs.com.  All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char rcsid[] = "$Tools: rand_write.c,v 1.0 2013-4-08 13:22 ezlibs.com Exp $";
#endif /* not lint */

// 校验？ 不校验仅写数据比较快
#define VERIFY 1

#define BSIZE   4*10*1024


/*10240*/
//#define MAX_FILE_SIZE 150*1024*1024
//#define MAX_FILE_SIZE 7*1024*1024 // 约1分钟 per turn
#define MAX_FILE_SIZE 143708/2*1024/6 // //143708K valid， we will  use 5 process

#define RAND_NUM_MIN 1
#define RAND_NUM_MAX RAND_NUM_MIN+BSIZE

int rand_buff(char **buf, int min_len, int max_len);

int main(int argc, char *argv[])
{
    //extern int errno;
    register int from, to, record, rcc, wcc_from, wcc_to;
    char *buf = NULL;
    time_t tt_start;
    time_t tt_end;
    size_t st_round=0;

    //int ii;
    int rand_num=0;

    // cal mask
#ifdef VERIFY
    __MD5_CTX context;
    __MD5_CTX context_out;
#endif
    srand((unsigned)time(NULL));
    rand_num = rand_buff(&buf, RAND_NUM_MIN, RAND_NUM_MAX);
    assert(buf);
    free(buf);

    if (argc < 2)
    {
#ifndef lint
        printf("%s %s\n", copyright, rcsid);
#endif

        printf("%s <source> <dest>\n", argv[0]);

        return -1;
    }

    time_t st_lastprint = 0;
    int run=1;

    while (run==1)
    {
        char cmd_str[256];
        sprintf(cmd_str, "rm -f %s %s", argv[1], argv[2]);
        system(cmd_str);
        from = open(argv[1], O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR|S_IROTH);
        to = open(argv[2], O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR|S_IROTH);

        size_t write_bytes_from = 0;
        size_t write_bytes_to = 0;

#ifdef VERIFY
        ez_MD5Init(&context);
#endif//VERIFY
        time(&tt_start);
        st_round++;
        //printf("round:%u\n", st_round);

        for (record = 0;; ++record)
        {
            rand_num = rand_buff(&buf, RAND_NUM_MIN, RAND_NUM_MAX);
            assert(buf);
            rcc = rand_num;

            ///////////////////
            wcc_from = write(from, buf, rcc);
            if (wcc_from == rcc)
            {
                write_bytes_from+=wcc_from;
                //printf("Record %8d: write: %8d, file %s\n", record, wcc_from, argv[1]);
            }
            else if (wcc_from < 0)
            {
                printf("Record %d: write error: errno=%d\n",
                       record, errno);
                free(buf);
                break;
            }
            else        if (wcc_from < rcc)
            {
                printf("Record %d: write short; expected %d, got %d\n",
                       record, rcc, wcc_from);
                free(buf);
                break;
            }

            ///////////////////
            wcc_to = write(to, buf, rcc);
            if (wcc_to == rcc)
            {
                write_bytes_to+=wcc_to;
                //printf("Record %8d: write: %8d, file %s\n", record, wcc_to, argv[2]);
            }
            else if (wcc_to < 0)
            {
                printf("Record %d: write error: errno=%d\n",
                       record, errno);
                free(buf);
                break;
            }
            else        if (wcc_to < rcc)
            {
                printf("Record %d: write short; expected %d, got %d\n",
                       record, rcc, wcc_to);
                free(buf);
                break;
            }
            //printf("Record %8d: write: %8d, total %u - %u\n", record, wcc_to, write_bytes_from, write_bytes_to);

            /////////////////////////////////
#ifdef VERIFY
            unsigned char outputtemp[16]={0};
            int iPrint;
            ez_MD5Update(&context, (unsigned char *)buf, rcc);
            memcpy(&context_out, &context, sizeof(__MD5_CTX));
            ez_MD5Final(outputtemp, &context_out);

            char md5str_buffer[FLIE_MD5_HASH_STR_LEN+1]={0};
            char szTemp[4] = {0};

            for (iPrint = 0; iPrint < 16; iPrint++)
            {
                //printf("%02X",outputtemp[iPrint]);
                sprintf(szTemp,"%02X",outputtemp[iPrint]);
                strcat((char *)md5str_buffer,szTemp);
            }
#endif
            int show = 0;
            //static time_t st_lastprint = time(NULL);
            if (time(NULL)-st_lastprint>10)// show msg per 5sec
            {
                show = 1;
                st_lastprint = time(NULL);
            }
            //            if (show)
            //                printf("MD5 checksum is ... ");

#ifdef VERIFY
            char md5str_file_1[FLIE_MD5_HASH_STR_LEN+1]={0};
            char md5str_file_2[FLIE_MD5_HASH_STR_LEN+1]={0};
            ez_file_md5(md5str_file_1, argv[1]);
            ez_file_md5(md5str_file_2, argv[2]);

            if ( (strcmp(md5str_buffer, md5str_file_1)==0)
                 &&strcmp(md5str_buffer, md5str_file_2)==0)
            {
                //if (show)
                //printf("correct:\n\t[%s][%s][%s]\n", md5str_buffer, md5str_file_1, md5str_file_2);
            }
            else
            {
                printf("incorrect:\n\t[%s][%s][%s]\n", md5str_buffer, md5str_file_1, md5str_file_2);
                pause();
            }
#endif
            ////////////////////////////

            free(buf);

            // c size
            if (write_bytes_from >= MAX_FILE_SIZE)
            {
                printf("exceeded %d, break now ...\n", MAX_FILE_SIZE);
                break;
            }
            else
            {
                // check data
            }
            if (show)
                printf("round %4ld running  : %4d records , %10ld Bytes written in %8ld seconds.\n", st_round , record, write_bytes_from+write_bytes_to, time(NULL)-tt_start);

            show = 0;

        }

        close(from);
        close(to);

        time(&tt_end);


#ifdef VERIFY

        printf("round %4ld completed: %4d records , %10ld Bytes written in %8ld seconds.\n", st_round , record, write_bytes_from+write_bytes_to, tt_end-tt_start);
        printf("---------------------------------------------------------------------------------------------------------\n");
#else //VERIFY

        printf("round %4ld completed: %4d records , %10ld Bytes written in %8ld seconds.\n", st_round , record, write_bytes_from+write_bytes_to, tt_end-tt_start);
        printf("---------------------------------------------------------------------------------------------------------\n");
        break;
#endif //VERIFY

    }
    ;// while run
    return 0;
}

int rand_buff(char **buf, int min_len, int max_len)
{
    int rand_num;
    int rand_buff_size;
    int ii;
    char *__buf;
    assert(max_len>min_len);

    rand_num = (rand() % (max_len-min_len+1))+ min_len;
    (*buf) = (char *)malloc(rand_num);
    __buf = (*buf);
    rand_buff_size = rand_num;

    for (ii=0; ii<rand_buff_size; ii++)
    {
        rand_num = (rand() % (127-0+1))+ 0;
        __buf[ii] = rand_num;
    }

    return rand_buff_size;
}
