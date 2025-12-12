#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sha1.h"

int main()
{
    {
        char *pstr = "e64c457940414e914448da413ae7ce8c8b2002dbd7acd6d9-7321-4535-8c54-a572fee5216c1471419588\n";
        char dst[SHA1HashSizeDestBuffer] = {0};
        int iret = ez_sha1_str(dst, sizeof(dst), pstr);

        printf ("%32s :: %d, dst %s.\n", "ez_sha1_str", iret, dst);
    }

    {
        char *pstr = "e64c457940414e914448da413ae7ce8c8b2002dbd7acd6d9-7321-4535-8c54-a572fee5216c1471419588\n";
        int len = strlen(pstr);
        char dst[SHA1HashSizeDestBuffer] = {0};

        int iret = ez_sha1(dst, sizeof(dst), pstr, len);

        printf ("%32s :: %d, dst %s.\n", "ez_sha1", iret, dst);
    }

    return 0;
}
