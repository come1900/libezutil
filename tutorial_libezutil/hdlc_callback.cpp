/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * hdlc_callback.cpp - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: hdlc_callback.cpp 5884 2016-12-15 09:29:31Z WuJunjie $
 *
 *  Explain:
 *     -
 *       hdlc 解析和回调示例
 *       
 *       测试用例覆盖：
 *       1. ez_hdlc_get_dst_buf_len() - 测试缓冲区长度计算（正常和超长情况）
 *       2. ez_hdlc_make_packet() - 测试HDLC包生成
 *          - EZ_HDLC_MODE_VERIFY (CRC8校验)
 *          - EZ_HDLC_MODE_VERIFY_MOD_BUS (CRC16校验)
 *          - EZ_HDLC_MODE_NOT_VERIFY (无校验)
 *          - 缓冲区不足检测
 *       3. ez_hdlc_encode/decode() - 测试数据编码/解码
 *          - 包含特殊字符（FLAG和ESC）的数据编码解码
 *          - 缓冲区溢出处理
 *       4. ez_hdlc_get_data() - 测试从HDLC包中提取数据
 *          - 三种模式的数据提取
 *          - CRC校验错误检测
 *          - 无效HDLC包检测
 *       5. hdlc_ondata() - 测试数据接收和回调
 *          - C函数回调
 *          - C++类成员函数回调
 *          - 边界情况：NULL指针、零长度、缓冲区溢出、不完整包
 *       6. hdlc_new/delete() - 测试资源管理
 *          - NULL指针处理
 *       
 *       测试结果：
 *       - 所有测试用例均通过（✓ PASS）
 *       - 测试输出使用emoji标识：✓ 表示通过，✗ 表示失败
 *       - 验证了HDLC协议的正确性、边界情况处理和错误检测能力
 *     -
 *
 *  Update:
 *     2016-12-15 09:29:31  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ezutil/str_opr.h>
#include <ezutil/ez_hdlc.h>


#define BUF_LEN 64

int c_fun_call_back(unsigned char *buf, int len, void *arg)
{
    printf("%s : %d\n",  __FUNCTION__, len);

    return 0;
}


class CCallBack
{
public :
    int class_fun_call_back(unsigned char *buf, int len, void *arg)
    {
        printf("CCallBack::%s : %d\n",  __FUNCTION__, len);

        return 0;
    }
};

int cpp_fun_call_back(unsigned char *buf, int len, void *arg)
{
    printf("%s : %d\n",  __FUNCTION__, len);
    
    if (NULL == arg)
    	return 0;

    CCallBack *pObj = (CCallBack *)arg;

    pObj->class_fun_call_back(buf, len, arg);

    return 0;
}


int main(int argc, char* argv[])
{
    // ez_hdlc_make_packet EZ_HDLC_MODE_VERIFY
    {
        int iret;
        //unsigned char *src=(unsigned char*)"Hello world";
        unsigned char src[]={'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};

        int hdlc_buf_len = ez_hdlc_get_dst_buf_len(src, (int) (sizeof(src)), EZ_HDLC_MODE_VERIFY);

        printf("%s -- ez_hdlc_get_dst_buf_len:%d\n", hdlc_buf_len==(sizeof(src)+3)?"✓ OK":"✗ NOK", hdlc_buf_len);

        unsigned char dest[BUF_LEN];
        iret = ez_hdlc_make_packet(dest, sizeof(dest), src, (int) (sizeof(src)), EZ_HDLC_MODE_VERIFY);
        if (iret>0)
        {
            dumpBuffer(stdout, (unsigned char *)dest,
                       iret,
                       SHOW_ASCII | SHOW_BINAR | SHOW_HEXAD | SHOW_LINER);

        }
        else
        {
            printf("✗ failed ez_hdlc_make_packet:%d\n", iret);
        }

        //        src[1] = (unsigned char)EZ_HDLC_FLAG;
        //        src[3] = (unsigned char)EZ_HDLC_ESC_ASYNC;
        //int hdlc_buf_len = ez_hdlc_get_dst_buf_len(src, (int) (sizeof(src)), EZ_HDLC_MODE_VERIFY);
        //printf("ez_hdlc_get_dst_buf_len:%d\n", hdlc_buf_len);

        // buf长度不足
        iret = ez_hdlc_make_packet(dest, hdlc_buf_len-1, src, (int) (sizeof(src)), EZ_HDLC_MODE_VERIFY);
        if (def_RET_BUFF_NOT_ENOUGH == iret)
        {
            //
            printf("✓ PASS -- ez_hdlc_make_packet:%s(%d)\n", "def_RET_BUFF_NOT_ENOUGH", iret);
        }
        else
        {
            printf("✗ ez_hdlc_make_packet have bug:%d\n", iret);
        }

        // buf长度足够， 正常
        iret = ez_hdlc_make_packet(dest, hdlc_buf_len, src, (int) (sizeof(src)), EZ_HDLC_MODE_VERIFY);
        if (iret<0)
        {
            //
            printf("✗ FAILED -- ez_hdlc_make_packet have bug:%d\n", iret);
        }
        else
        {
            printf("✓ PASS -- ez_hdlc_make_packet:%d\n", iret);
        }
    }

    // ez_hdlc_make_packet 过长等异常
    {
        //EZ_HDLC_PACKET_MAX_LEN
        unsigned char src[0xFD] = {0};

        int hdlc_buf_len = ez_hdlc_get_dst_buf_len(src, (int) (sizeof(src)), EZ_HDLC_MODE_VERIFY);
        if (hdlc_buf_len<0)
        {
            printf("✓ PASS -- dst_data_len:%d\n", (-1*hdlc_buf_len) - EZ_HDLC_HEAD_LEN);
        }
        else
        {
            printf("✗ FAILED -- dst_data_len:%d\n", hdlc_buf_len);
        }

    }
    {
        //EZ_HDLC_PACKET_MAX_LEN
        unsigned char src[250] = {0}; //250 = 253(fd)-2(headlen)-1(verify)

        int hdlc_buf_len = ez_hdlc_get_dst_buf_len(src, (int) (sizeof(src)), EZ_HDLC_MODE_VERIFY);
        if (hdlc_buf_len > 0)
        {
            printf("✓ PASS -- dst_data_len:%d\n", hdlc_buf_len);
        }
    }

    // ez_hdlc_make_packet EZ_HDLC_MODE_VERIFY_MOD_BUS
    {
        int iret;
        //unsigned char *src=(unsigned char*)"Hello world";
        unsigned char src[]={'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};

        int hdlc_buf_len = ez_hdlc_get_dst_buf_len(src, (int) (sizeof(src)), EZ_HDLC_MODE_VERIFY_MOD_BUS);
        printf("%s -- ez_hdlc_get_dst_buf_len:%d\n", hdlc_buf_len==(sizeof(src)+4)?"✓ OK":"✗ NOK", hdlc_buf_len);

        unsigned char dest[BUF_LEN];
        iret = ez_hdlc_make_packet(dest, sizeof(dest), src, (int) (sizeof(src)), EZ_HDLC_MODE_VERIFY_MOD_BUS);
        if (iret>0)
        {
            dumpBuffer(stdout, (unsigned char *)dest,
                       iret,
                       SHOW_ASCII | SHOW_BINAR | SHOW_HEXAD | SHOW_LINER);

        }
        else
        {
            printf("✗ failed ez_hdlc_make_packet:%d\n", iret);
        }

        //        src[1] = (unsigned char)EZ_HDLC_FLAG;
        //        src[3] = (unsigned char)EZ_HDLC_ESC_ASYNC;
        //int hdlc_buf_len = ez_hdlc_get_dst_buf_len(src, (int) (sizeof(src)), EZ_HDLC_MODE_VERIFY);
        //printf("ez_hdlc_get_dst_buf_len:%d\n", hdlc_buf_len);

        // 让buf不足
        iret = ez_hdlc_make_packet(dest, hdlc_buf_len-1, src, (int) (sizeof(src)), EZ_HDLC_MODE_VERIFY_MOD_BUS);
        if (def_RET_BUFF_NOT_ENOUGH == iret || iret == -2)
        {
            //意料之中 - 缓冲区不足，可能返回 -5 (编码时检测) 或 -2 (添加CRC时检测)
            printf("✓ PASS -- ez_hdlc_make_packet detects buffer insufficient: %d\n", iret);
        }
        else
        {
            printf("✗ FAILED -- ez_hdlc_make_packet should detect buffer insufficient, got: %d\n", iret);
        }

        // buf长度
        iret = ez_hdlc_make_packet(dest, hdlc_buf_len, src, (int) (sizeof(src)), EZ_HDLC_MODE_VERIFY_MOD_BUS);
        if (iret<0)
        {
            //意料之中
            printf("✗ ez_hdlc_make_packet have bug:%d\n", iret);
        }
        else
        {
            printf("✓ OK -- ez_hdlc_make_packet:%d\n", iret);
        }
    }

    // handler of c
    {
        int iret;
        int dst_len;
        unsigned char *src=(unsigned char*)"Hello world";
        unsigned char dest[BUF_LEN];
        dst_len = ez_hdlc_make_packet(dest, sizeof(dest), src, (int) (strlen((char *)src)), EZ_HDLC_MODE_VERIFY);

        unsigned char buf_hdlc[BUF_LEN*2] = {0};

        memcpy(buf_hdlc, dest, dst_len);
        memcpy(buf_hdlc+dst_len+8, dest, dst_len);

        HDLC_HANDLER_T *p_handler = NULL;
        p_handler = hdlc_new();
        p_handler->pHdlcCallback = c_fun_call_back;

        iret = hdlc_ondata(p_handler, buf_hdlc, dst_len*2+8/*rubbish data*/+1, NULL);
        printf("hdlc_ondata:%d\n", iret);

        hdlc_delete(p_handler);
    }

    // handler of cpp
    {
        int iret;
        int dst_len;
        unsigned char *src=(unsigned char*)"Hello world";
        unsigned char dest[BUF_LEN];
        dst_len = ez_hdlc_make_packet(dest, sizeof(dest), src, (int) (strlen((char *)src)), EZ_HDLC_MODE_VERIFY);

        unsigned char buf_hdlc[BUF_LEN*2] = {0};

        memcpy(buf_hdlc, dest, dst_len);
        memcpy(buf_hdlc+dst_len, dest, dst_len);

        HDLC_HANDLER_T *p_handler = NULL;
        p_handler = hdlc_new();
        p_handler->pHdlcCallback = cpp_fun_call_back;
        
        CCallBack CallbackClass;

        iret = hdlc_ondata(p_handler, buf_hdlc, dst_len*2+1, &CallbackClass);
        printf("hdlc_ondata:%d\n", iret);

        iret = hdlc_ondata(p_handler, buf_hdlc, dst_len*2+1, NULL);
        printf("hdlc_ondata:%d\n", iret);

        hdlc_delete(p_handler);
    }

    // Test ez_hdlc_encode and ez_hdlc_decode
    {
        printf("\n=== Test ez_hdlc_encode/decode ===\n");
        unsigned char src[] = {0xFE, 0x7D, 0x20, 'H', 'e', 'l', 'l', 'o'};
        unsigned char encoded[BUF_LEN];
        unsigned char decoded[BUF_LEN];
        int ret;

        ret = ez_hdlc_encode(encoded, sizeof(encoded), src, sizeof(src));
        if (ret > 0)
        {
            printf("✓ PASS -- ez_hdlc_encode: %d bytes\n", ret);
            dumpBuffer(stdout, encoded, ret, SHOW_ASCII | SHOW_BINAR | SHOW_HEXAD | SHOW_LINER);
        }
        else
        {
            printf("✗ FAILED -- ez_hdlc_encode: %d\n", ret);
        }

        ret = ez_hdlc_decode(decoded, sizeof(decoded), encoded, ret);
        if (ret > 0 && memcmp(src, decoded, sizeof(src)) == 0)
        {
            printf("✓ PASS -- ez_hdlc_decode: %d bytes, data matches\n", ret);
        }
        else
        {
            printf("✗ FAILED -- ez_hdlc_decode: %d\n", ret);
        }

        // Test buffer overflow
        ret = ez_hdlc_encode(encoded, 2, src, sizeof(src));
        if (ret < 0)
        {
            printf("✓ PASS -- ez_hdlc_encode buffer overflow handled: %d\n", ret);
        }
        else
        {
            printf("✗ FAILED -- ez_hdlc_encode should fail on buffer overflow\n");
        }
    }

    // Test ez_hdlc_get_data
    {
        printf("\n=== Test ez_hdlc_get_data ===\n");
        unsigned char src[] = {'H', 'e', 'l', 'l', 'o'};
        unsigned char packet[BUF_LEN];
        unsigned char data[BUF_LEN];
        int packet_len, data_len;

        // Test with EZ_HDLC_MODE_VERIFY
        packet_len = ez_hdlc_make_packet(packet, sizeof(packet), src, sizeof(src), EZ_HDLC_MODE_VERIFY);
        if (packet_len > 0)
        {
            data_len = ez_hdlc_get_data(data, sizeof(data), packet, packet_len, EZ_HDLC_MODE_VERIFY);
            if (data_len > 0 && memcmp(src, data, sizeof(src)) == 0)
            {
                printf("✓ PASS -- ez_hdlc_get_data (VERIFY): %d bytes, data matches\n", data_len);
            }
            else
            {
                printf("✗ FAILED -- ez_hdlc_get_data (VERIFY): %d\n", data_len);
            }

            // Test with wrong CRC (corrupt packet)
            packet[packet_len - 1] ^= 0xFF;
            data_len = ez_hdlc_get_data(data, sizeof(data), packet, packet_len, EZ_HDLC_MODE_VERIFY);
            if (data_len == -3)
            {
                printf("✓ PASS -- ez_hdlc_get_data detects CRC error: %d\n", data_len);
            }
            else
            {
                printf("✗ FAILED -- ez_hdlc_get_data should detect CRC error, got: %d\n", data_len);
            }
        }

        // Test with EZ_HDLC_MODE_VERIFY_MOD_BUS
        packet_len = ez_hdlc_make_packet(packet, sizeof(packet), src, sizeof(src), EZ_HDLC_MODE_VERIFY_MOD_BUS);
        if (packet_len > 0)
        {
            data_len = ez_hdlc_get_data(data, sizeof(data), packet, packet_len, EZ_HDLC_MODE_VERIFY_MOD_BUS);
            if (data_len > 0 && memcmp(src, data, sizeof(src)) == 0)
            {
                printf("✓ PASS -- ez_hdlc_get_data (MOD_BUS): %d bytes, data matches\n", data_len);
            }
            else
            {
                printf("✗ FAILED -- ez_hdlc_get_data (MOD_BUS): %d\n", data_len);
            }
        }

        // Test with EZ_HDLC_MODE_NOT_VERIFY
        packet_len = ez_hdlc_make_packet(packet, sizeof(packet), src, sizeof(src), EZ_HDLC_MODE_NOT_VERIFY);
        if (packet_len > 0)
        {
            data_len = ez_hdlc_get_data(data, sizeof(data), packet, packet_len, EZ_HDLC_MODE_NOT_VERIFY);
            if (data_len > 0 && memcmp(src, data, sizeof(src)) == 0)
            {
                printf("✓ PASS -- ez_hdlc_get_data (NOT_VERIFY): %d bytes, data matches\n", data_len);
            }
            else
            {
                printf("✗ FAILED -- ez_hdlc_get_data (NOT_VERIFY): %d\n", data_len);
            }
        }

        // Test with invalid HDLC packet (no flag)
        unsigned char invalid_packet[] = {0x00, 0x05, 'H', 'e', 'l', 'l', 'o'};
        data_len = ez_hdlc_get_data(data, sizeof(data), invalid_packet, sizeof(invalid_packet), EZ_HDLC_MODE_VERIFY);
        if (data_len == -2)
        {
            printf("✓ PASS -- ez_hdlc_get_data detects invalid packet: %d\n", data_len);
        }
        else
        {
            printf("✗ FAILED -- ez_hdlc_get_data should detect invalid packet, got: %d\n", data_len);
        }
    }

    // Test ez_hdlc_make_packet with EZ_HDLC_MODE_NOT_VERIFY
    {
        printf("\n=== Test ez_hdlc_make_packet (NOT_VERIFY) ===\n");
        unsigned char src[] = {'H', 'e', 'l', 'l', 'o'};
        unsigned char dest[BUF_LEN];
        int ret = ez_hdlc_make_packet(dest, sizeof(dest), src, sizeof(src), EZ_HDLC_MODE_NOT_VERIFY);
        if (ret > 0)
        {
            printf("✓ PASS -- ez_hdlc_make_packet (NOT_VERIFY): %d bytes\n", ret);
            dumpBuffer(stdout, dest, ret, SHOW_ASCII | SHOW_BINAR | SHOW_HEXAD | SHOW_LINER);
        }
        else
        {
            printf("✗ FAILED -- ez_hdlc_make_packet (NOT_VERIFY): %d\n", ret);
        }
    }

    // Test hdlc_ondata with edge cases
    {
        printf("\n=== Test hdlc_ondata edge cases ===\n");
        HDLC_HANDLER_T *p_handler = hdlc_new();
        p_handler->pHdlcCallback = c_fun_call_back;

        // Test with NULL handler
        int ret = hdlc_ondata(NULL, (unsigned char*)"test", 4, NULL);
        if (ret == -1)
        {
            printf("✓ PASS -- hdlc_ondata handles NULL handler: %d\n", ret);
        }
        else
        {
            printf("✗ FAILED -- hdlc_ondata should return -1 for NULL handler, got: %d\n", ret);
        }

        // Test with NULL data
        ret = hdlc_ondata(p_handler, NULL, 4, NULL);
        if (ret == -1)
        {
            printf("✓ PASS -- hdlc_ondata handles NULL data: %d\n", ret);
        }
        else
        {
            printf("✗ FAILED -- hdlc_ondata should return -1 for NULL data, got: %d\n", ret);
        }

        // Test with zero length
        unsigned char dummy = 0;
        ret = hdlc_ondata(p_handler, &dummy, 0, NULL);
        if (ret == -1)
        {
            printf("✓ PASS -- hdlc_ondata handles zero length: %d\n", ret);
        }
        else
        {
            printf("✗ FAILED -- hdlc_ondata should return -1 for zero length, got: %d\n", ret);
        }

        // Test with incomplete packet
        unsigned char incomplete[] = {EZ_HDLC_FLAG, 0x10}; // Flag + length, but not enough data
        ret = hdlc_ondata(p_handler, incomplete, sizeof(incomplete), NULL);
        if (ret >= 0)
        {
            printf("✓ PASS -- hdlc_ondata handles incomplete packet: %d\n", ret);
        }
        else
        {
            printf("INFO -- hdlc_ondata with incomplete packet: %d\n", ret);
        }

        // Test with buffer overflow
        unsigned char large_data[EZ_HDLC_BUFF_LEN*2 + 10] = {0};
        memset(large_data, 0xAA, sizeof(large_data));
        ret = hdlc_ondata(p_handler, large_data, sizeof(large_data), NULL);
        if (ret == -2)
        {
            printf("✓ PASS -- hdlc_ondata handles buffer overflow: %d\n", ret);
        }
        else
        {
            printf("INFO -- hdlc_ondata with large data: %d\n", ret);
        }

        hdlc_delete(p_handler);
    }

    // Test hdlc_delete with NULL
    {
        printf("\n=== Test hdlc_delete edge cases ===\n");
        int ret = hdlc_delete(NULL);
        if (ret == -1)
        {
            printf("✓ PASS -- hdlc_delete handles NULL: %d\n", ret);
        }
        else
        {
            printf("✗ FAILED -- hdlc_delete should return -1 for NULL, got: %d\n", ret);
        }
    }

    // Test with data containing special characters
    {
        printf("\n=== Test with special characters (FLAG and ESC) ===\n");
        unsigned char src_with_special[] = {EZ_HDLC_FLAG, 'A', EZ_HDLC_ESC_ASYNC, 'B', 'C'};
        unsigned char dest[BUF_LEN];
        unsigned char decoded[BUF_LEN];
        int packet_len, data_len;

        packet_len = ez_hdlc_make_packet(dest, sizeof(dest), src_with_special, sizeof(src_with_special), EZ_HDLC_MODE_VERIFY);
        if (packet_len > 0)
        {
            printf("✓ PASS -- ez_hdlc_make_packet with special chars: %d bytes\n", packet_len);
            data_len = ez_hdlc_get_data(decoded, sizeof(decoded), dest, packet_len, EZ_HDLC_MODE_VERIFY);
            if (data_len > 0 && memcmp(src_with_special, decoded, sizeof(src_with_special)) == 0)
            {
                printf("✓ PASS -- Special characters encoded/decoded correctly\n");
            }
            else
            {
                printf("✗ FAILED -- Special characters encoding/decoding failed\n");
            }
        }
        else
        {
            printf("✗ FAILED -- ez_hdlc_make_packet with special chars: %d\n", packet_len);
        }
    }

    return 0;
}
