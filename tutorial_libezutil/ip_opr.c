//
//  $Id: ip_opr.c 0001 2017-12-15 04:21:06 WuJunjie Exp $
//
// Description: Function
// Revisions: $Id: ip_opr.c 0001 2017-12-15 04:21:06 WuJunjie Exp $
//

//exa :
//$ ./ip_opr.o 
//192.168.1.2 and 192.168.1.3 : 1 
//192.168.2.2 and 192.168.1.3 : 0 

#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    char *ip_str_phone  =  "192.168.1.2";
    char *ip_str_ipc    =  "192.168.1.3";
    char *ip_str_mask   =  "255.255.255.0";

    struct in_addr in_addr_phone;
    struct in_addr in_addr_ipc;
    struct in_addr in_addr_mask;

    inet_aton(ip_str_phone, &in_addr_phone);
    inet_aton(ip_str_mask, &in_addr_mask);
    inet_aton(ip_str_ipc, &in_addr_ipc);

    printf("%s and %s : %d \n", ip_str_phone, ip_str_ipc, (in_addr_phone.s_addr&in_addr_mask.s_addr) == (in_addr_ipc.s_addr&in_addr_mask.s_addr) );

    ip_str_phone = "192.168.2.2";
    inet_aton(ip_str_phone, &in_addr_phone);
    printf("%s and %s : %d \n", ip_str_phone, ip_str_ipc, (in_addr_phone.s_addr&in_addr_mask.s_addr) == (in_addr_ipc.s_addr&in_addr_mask.s_addr) );

    return 0;
}
