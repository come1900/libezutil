/* main.c */

#include <stdlib.h>
#include <stdio.h>

#include "JSON_checker.h"

int main(int argc, char* argv[])
{
    int iret = 0;
    char *pJsonString = NULL;

    iret = json_checker(pJsonString, 0);
    printf("%6d, json_checker:%s\n", iret, "NULL");

    pJsonString = "{\"a\" : 1}";
    iret = json_checker(pJsonString, 0);
    printf("%6d, json_checker:%s\n", iret, pJsonString);

    pJsonString = "{\"a : 1}";
    iret = json_checker(pJsonString, 0);
    printf("%6d, json_checker:%s\n", iret, pJsonString);

    pJsonString = "{\"a\" : }";
    iret = json_checker(pJsonString, 0);
    printf("%6d, json_checker:%s\n", iret, pJsonString);

    int ii=0;
    do
    {
        if (0 == ii%99999)
            printf(" -- >> %6d\n", ii);

        pJsonString = "{\"ID\":\"000000006b4aee41016b503718830012\",\"Type\":\"3\",\"ValidPeriod\":\"0\",\"Storage\":{\"Channel\":\"2\",\"Description\":\"nvr_dh\",\"Name\":\"nvr://admin:B5DD4F86@192.168.9.137:37777/sreq?http=8090&rtsp=554&onvif=8090\",\"Type\":\"3\"},\"Stream\":{\"Account\":\"admin\",\"Passwd\":\"B5DD4F86\",\"Channel\":\"1\",\"Subtype\":\"1\",\"Location\":\"010-111\",\"Url\":\"pss://192.168.9.137:37777/sreq?http=8090&rtsp=554\",\"Description\":\"KW12\",\"VideoEncode\":\"0\",\"VideoRefps\":\"0\",\"VideoResize\":\"0\",\"AudioEncode\":\"0\"}}";
        iret = json_checker(pJsonString, 0);

        ++ii;
    }
    while(1);

    return 0;
}
