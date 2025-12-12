
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ezutil/str_opr.h>

int main(int argc, char *argv[])
{
    char buff[1024] = {'\0'};

    strcpy(buff, "i love joke, i love funny");

    //构造一个内部还有的
    char *from = "love";
    char *to = "do not love";
    char *to2 = "like";

    printf("buff:%s\n", buff);

    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

    char *pBuf = replace_chars_with_chars(buff, from, to);
    printf("%s:", "replace_chars_with_chars");

    if (pBuf == NULL)
    {
        printf("%s\n", "failed!");
    }
    else
    {
        printf("%s\n", pBuf);
    }
    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    pBuf = replace_chars_with_chars(buff, from, to2);
    printf("%s:", "replace_chars_with_chars");

    if (pBuf == NULL)
    {
        printf("%s\n", "failed!");
    }
    else
    {
        printf("%s\n", pBuf);
    }

    return 0;
}
