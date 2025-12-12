#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ezutil/sha1.h"

int main(int argc, char *argv[])
{
    int iret;
    //char *pstr = "e64c457940414e914448da413ae7ce8c8b2002dbd7acd6d9-7321-4535-8c54-a572fee5216c1471419588\n";
    char *pstr = "PUT\n\n\nFri, 14 Dec 2018 13:27:00 GMT\n/js_tst/folder1/uploadFile.sh";
    pstr = "a";
    char dst[SHA1HashSize*2+1] = {0};

    if (argc>1)
    {
        pstr = argv[1];
    }
    else
    {
        printf("usage: %s [str]\n", argv[0]);
        printf("default:[%s]\n", pstr);
    }

    printf("ez_sha1_str:[%s] -->\n", pstr);
    iret = ez_sha1_str(dst, sizeof(dst), pstr);
    if (iret >= 0)
    {
        printf ("\t dst:[%s]\n", dst);
    }
    else
    {
        printf ("\t ez_sha1_str failed:%d\n", iret);
    }

    return 0;
}

