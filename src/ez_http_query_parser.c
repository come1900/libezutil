/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ez_http_query_parser.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ez_http_query_parser.c 5884 2019-08-15 11:08:39Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2019-08-15 11:08:39  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ez_http_query_parser.h"

// Helper macro to convert two-character hex strings to character value
#define ToHex_ez_getParam_HttpQuery(Y) (Y>='0'&&Y<='9'?Y-'0':Y-'A'+10)

int ez_getParam_HttpQuery(const char *InputData, const char *Name, char *Value)
{
    char *pos1 = (char *)strstr(InputData, Name);

    if (pos1)
    {
        pos1 += strlen(Name);

        if (*pos1 == '=')
        { // Make sure there is an '=' where we expect it
            pos1++;

            while (*pos1 && *pos1 != '&')
            {
                if (*pos1 == '%')
                { // Convert it to a single ASCII character and store at our Valueination
                    *Value++ = (char)ToHex_ez_getParam_HttpQuery(pos1[1]) * 16 + ToHex_ez_getParam_HttpQuery(pos1[2]);
                    pos1 += 3;
                }
                else if( *pos1=='+' )
                { // If it's a '+', store a space at our Valueination
                    *Value++ = ' ';
                    pos1++;
                }
                else
                {
                    *Value++ = *pos1++; // Otherwise, just store the character at our Valueination
                }
            }

            *Value++ = '\0';
            return 0;
        }

    }

    *Value = '\0';

    return 0;
}
