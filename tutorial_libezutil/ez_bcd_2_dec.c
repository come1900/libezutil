
#include <stdio.h>
#include <stdlib.h>

#include <ezutil/str_opr.h>


int main(int argc, char *argv[])
{
    {
        unsigned char buff[] = {0x20, 0x18};
        unsigned long year=ez_bcd_2_dec(buff, sizeof(buff));
        printf("year:%ld\n", year);
    }

    {
        unsigned char buff[] = {0x18};
        unsigned long year=ez_bcd_2_dec(buff, sizeof(buff));
        printf("year:%ld\n", year);
    }

    return 0;
}
