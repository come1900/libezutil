//
//  $Id: http_parse_and_callback.c 0001 2012-07-10 03:49:22 WuJunjie Exp $
//
//  Copyright (C) 2012 ezlibs.com, All Rights Reserved.
//
// Description: http解析， 通过callback解析 
// Revisions: $Id: http_parse_and_callback.c 0001 2012-07-10 03:49:22 WuJunjie Exp $
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ezutil/http_parser.h>


#define BUF_LEN 1000
#define DCB_CALLBACK(x) int http_##x##_callback(http_parser *parser, const char *at, size_t length)

DCB_CALLBACK(header_field) {    
#define CODES \
    char str_buf[BUF_LEN];\
    memcpy(str_buf, at, length);\
    str_buf[length] = '\0';

    CODES
    printf("%s : ", str_buf);
    return 0;
}

DCB_CALLBACK(header_value) {
    CODES
    printf("%s\n", str_buf);
    return 0;
}

DCB_CALLBACK(body) {
    CODES
    printf("body : \n%s\n", str_buf);
    return 0;
}

DCB_CALLBACK(url) {
    CODES
    printf("url : %s\n", str_buf);
    return 0;
}

#define CB_CALLBACK(x) int http_##x##_callback(http_parser *parser)
#define TOKEN "==================================="
CB_CALLBACK(message_begin) {
    printf("\n\nparser starts working , result below : \n"TOKEN"\n");
    return 0;
}

CB_CALLBACK(message_complete) {
    printf(TOKEN"\nparser ends\n");
    return 0;
}

CB_CALLBACK(headers_complete) {
    printf(TOKEN"\n");
    return 0;
}


int main(int argc, char* argv[]) {
    const char *http_req = 
    "GET http://cis.baidu.com/cis/cis.txt HTTP/1.1\r\n"
    "Accept: */*\r\n"
    "Accept-Encoding: gzip, deflate\r\n"
    "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729)\r\n"
    "Host: cis.baidu.com\r\n"
    "Connection: Keep-Alive\r\n"
    "Cookie: BAIDUID=1D87E461519AD9EB47411775459F0634:FG=1;\r\n\r\n";
    
    const char *http_res = 
    "HTTP/1.1 200 OK\r\n"
    "Server: nginx\r\n"
    "Date: Wed, 07 Mar 2012 09:43:02 GMT\r\n"
    "Content-Type: text/html; charset=utf-8\r\n"
    "Connection: keep-alive\r\n"
    "Vary: Accept-Encoding\r\n"
    "Content-Encoding: gzip\r\n"
    "Content-Length: 20\r\n\r\n12345678901234567890";

    http_parser *parser = (http_parser*)malloc(sizeof(http_parser));
    http_parser_settings *settings = (http_parser_settings*) malloc(sizeof(http_parser_settings));    
    #define NCB(n) http_##n##_callback
    #define SCB(x) settings->on_##x = NCB(x)
    SCB(url);
    SCB(header_field);
    SCB(header_value);
    SCB(body);
    SCB(message_begin);
    SCB(headers_complete);
    SCB(message_complete);

    http_parser_init(parser, HTTP_REQUEST);
    http_parser_execute(parser, settings, http_req, strlen(http_req));
    
    
    //printf("parser starts working , result below : \n"TOKEN"\n");
    http_parser_init(parser, HTTP_RESPONSE);

    #define BIG_ENOUGH_BUF_LEN 10000
    int ret = http_parser_execute(parser, settings, http_res, BIG_ENOUGH_BUF_LEN);
    printf("\nret : %d\n", ret);

    return 0;
}
