
#include <stdio.h>
#include <stdlib.h>

#include <ezutil/str_opr.h>

int main(int argc, char *argv[])
{

    {
        char *pIp = "192.168.1.1";
        int isP = -1;

        isP = isPrivateIPAddress(pIp);
        printf("%16s : Private:%c\n", pIp, isP==1?'Y':'N');
    }
    {
        char *pIp = "172.16.1.1";
        int isP = -1;

        isP = isPrivateIPAddress(pIp);
        printf("%16s : Private:%c\n", pIp, isP==1?'Y':'N');
    }
    {
        char *pIp = "172.31.1.1";
        int isP = -1;

        isP = isPrivateIPAddress(pIp);
        printf("%16s : Private:%c\n", pIp, isP==1?'Y':'N');
    }
    {
        char *pIp = "10.10.10.1";
        int isP = -1;

        isP = isPrivateIPAddress(pIp);
        printf("%16s : Private:%c\n", pIp, isP==1?'Y':'N');
    }
    {
        char *pIp = "172.32.1.1";
        int isP = -1;

        isP = isPrivateIPAddress(pIp);
        printf("%16s : Private:%c\n", pIp, isP==1?'Y':'N');
    }
    {
        char *pIp = "11.16.1.1";
        int isP = -1;

        isP = isPrivateIPAddress(pIp);
        printf("%16s : Private:%c\n", pIp, isP==1?'Y':'N');
    }

    {
        // Sifa of China 
        char *pIp = "2.126.114.18";
        int isP = -1;

        isP = isPrivateIPAddress(pIp);
        printf("%16s : Private:%c\n", pIp, isP==1?'Y':'N');
    }

    return 0;
}
