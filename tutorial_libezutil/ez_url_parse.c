/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ez_url_parse.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ez_url_parse.c 5884 2016-11-23 04:50:49Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2016-11-23 04:50:49  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

//./ez_url_parse.o http://61.160.149.159:18080/hls/44c11db22c1248f23bf08dac00e4c630bcbac07f.m3u8?s=84216bd489eaca1ded7cb6c0b7480428670a2012
//./ez_url_parse.o 'http://61.160.149.159:18080/hls/44c11db22c1248f23bf08dac00e4c630bcbac07f.m3u8#fragment'
//./ez_url_parse.o 'rtsp://admin:admin@221.226.150.233:554/cam/realmonitor?channel=1&subtype=0'

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ezutil/ez_url_parser.h>

int main(int argc, char **argv)
{
    int i;

    ez_parsed_url_t *parsed_url;

    if (argc == 1)
    {
        fprintf(stderr, "No URL passed.\n");
        return 1;
    }

    for (i = 1; i < argc; i++)
    {

        parsed_url = ez_parse_url(argv[i]);

        if (parsed_url)
        {
            printf("Scheme: '%s'\nHost: '%s'\nPort: '%s'\nquery: '%s'\nPath: '%s'\nFragment: '%s'\nUsername: '%s'\nPassword: '%s'\n"
                   , parsed_url->scheme
                   , parsed_url->host
                   , parsed_url->port
                   , parsed_url->query
                   , parsed_url->path
                   , parsed_url->fragment
                   , parsed_url->username
                   , parsed_url->password
                   );

            ez_parsed_url_free(parsed_url);
        }
    }

    return 0;

}

