/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * hdlc_performance.cpp - HDLC性能测试
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: hdlc_performance.cpp 5884 2016-12-15 09:29:31Z WuJunjie $
 *
 *  Explain:
 *     -
 *       HDLC编码和解码性能测试
 *       测试每秒执行编码和解码的次数
 *     -
 *
 *  Update:
 *     2016-12-15 09:29:31  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <ezutil/ez_hdlc.h>

#define BUF_LEN EZ_HDLC_PACKET_MAX_LEN
#define TEST_DURATION_SEC 5  // 测试持续时间（秒）

int c_fun_call_back(unsigned char *buf, int len, void *arg)
{
    // 性能测试中不输出，减少I/O开销
    return 0;
}

int main(int argc, char* argv[])
{
    int test_duration = TEST_DURATION_SEC;
    if (argc > 1)
    {
        test_duration = atoi(argv[1]);
        if (test_duration <= 0)
        {
            test_duration = TEST_DURATION_SEC;
        }
    }

    unsigned char src[BUF_LEN];
    unsigned char dest[BUF_LEN*2];
    unsigned char decoded[BUF_LEN];
    
    // 初始化测试数据
    memset(src, 0xAA, sizeof(src));
    
    HDLC_HANDLER_T *p_handler = hdlc_new();
    p_handler->pHdlcCallback = c_fun_call_back;

    struct timespec start_time, end_time;
    long long encode_count = 0;
    long long decode_count = 0;
    int src_len;
    int dst_len;
    int data_len;

    printf("HDLC Performance Test (Duration: %d seconds)\n", test_duration);
    printf("==========================================\n\n");

    src_len = BUF_LEN / 2;  // 使用中等大小的数据包
    
    // 先准备一个包以获取包大小
    dst_len = ez_hdlc_make_packet(dest, sizeof(dest), src, src_len, EZ_HDLC_MODE_VERIFY);
    int packet_size = dst_len;  // 保存包大小用于计算
    
    // 测试编码性能
    printf("Testing Encode Performance...\n");
    
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    end_time = start_time;
    end_time.tv_sec += test_duration;
    
    long long encode_bytes = 0;
    int last_sec = -1;
    long long last_count = 0;
    long long last_bytes = 0;
    struct timespec last_print_time = start_time;
    
    while (1)
    {
        struct timespec current_time;
        clock_gettime(CLOCK_MONOTONIC, &current_time);
        
        int elapsed_sec = current_time.tv_sec - start_time.tv_sec;
        if (elapsed_sec > last_sec && elapsed_sec > 0 && elapsed_sec <= test_duration)
        {
            double elapsed = (current_time.tv_sec - last_print_time.tv_sec) + 
                           (current_time.tv_nsec - last_print_time.tv_nsec) / 1000000000.0;
            long long count_diff = encode_count - last_count;
            long long bytes_diff = encode_bytes - last_bytes;
            double msgs_per_sec = count_diff / elapsed;
            double mb_per_sec = (bytes_diff / 1024.0 / 1024.0) / elapsed;
            
            printf("  [%3d sec] Total: %12lld msgs, %12lld bytes | Speed: %10.2f msgs/sec, %8.2f MB/sec\n", 
                   elapsed_sec, encode_count, encode_bytes, msgs_per_sec, mb_per_sec);
            
            last_sec = elapsed_sec;
            last_count = encode_count;
            last_bytes = encode_bytes;
            last_print_time = current_time;
        }
        
        if (current_time.tv_sec > end_time.tv_sec || 
            (current_time.tv_sec == end_time.tv_sec && current_time.tv_nsec >= end_time.tv_nsec))
        {
            break;
        }
        
        dst_len = ez_hdlc_make_packet(dest, sizeof(dest), src, src_len, EZ_HDLC_MODE_VERIFY);
        if (dst_len > 0)
        {
            encode_count++;
            encode_bytes += dst_len;
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double encode_time = (end_time.tv_sec - start_time.tv_sec) + 
                         (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0;
    double encode_per_sec = encode_count / encode_time;
    double encode_mb_per_sec = (encode_bytes / 1024.0 / 1024.0) / encode_time;
    printf("  Encode Result: %lld msgs, %lld bytes, %.2f sec | Avg: %.2f msgs/sec, %.2f MB/sec\n\n", 
           encode_count, encode_bytes, encode_time, encode_per_sec, encode_mb_per_sec);

    // 测试解码性能
    printf("Testing Decode Performance...\n");
    
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    end_time = start_time;
    end_time.tv_sec += test_duration;
    
    long long decode_bytes = 0;
    last_sec = -1;
    last_count = 0;
    last_bytes = 0;
    last_print_time = start_time;
    
    while (1)
    {
        struct timespec current_time;
        clock_gettime(CLOCK_MONOTONIC, &current_time);
        
        int elapsed_sec = current_time.tv_sec - start_time.tv_sec;
        if (elapsed_sec > last_sec && elapsed_sec > 0 && elapsed_sec <= test_duration)
        {
            double elapsed = (current_time.tv_sec - last_print_time.tv_sec) + 
                           (current_time.tv_nsec - last_print_time.tv_nsec) / 1000000000.0;
            long long count_diff = decode_count - last_count;
            long long bytes_diff = decode_bytes - last_bytes;
            double msgs_per_sec = count_diff / elapsed;
            double mb_per_sec = (bytes_diff / 1024.0 / 1024.0) / elapsed;
            
            printf("  [%3d sec] Total: %12lld msgs, %12lld bytes | Speed: %10.2f msgs/sec, %8.2f MB/sec\n", 
                   elapsed_sec, decode_count, decode_bytes, msgs_per_sec, mb_per_sec);
            
            last_sec = elapsed_sec;
            last_count = decode_count;
            last_bytes = decode_bytes;
            last_print_time = current_time;
        }
        
        if (current_time.tv_sec > end_time.tv_sec || 
            (current_time.tv_sec == end_time.tv_sec && current_time.tv_nsec >= end_time.tv_nsec))
        {
            break;
        }
        
        data_len = ez_hdlc_get_data(decoded, sizeof(decoded), dest, packet_size, EZ_HDLC_MODE_VERIFY);
        if (data_len > 0)
        {
            decode_count++;
            decode_bytes += packet_size;
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double decode_time = (end_time.tv_sec - start_time.tv_sec) + 
                         (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0;
    double decode_per_sec = decode_count / decode_time;
    double decode_mb_per_sec = (decode_bytes / 1024.0 / 1024.0) / decode_time;
    printf("  Decode Result: %lld msgs, %lld bytes, %.2f sec | Avg: %.2f msgs/sec, %.2f MB/sec\n\n", 
           decode_count, decode_bytes, decode_time, decode_per_sec, decode_mb_per_sec);

    // 测试完整流程（编码+解码）
    printf("Testing Encode+Decode Performance...\n");
    
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    end_time = start_time;
    end_time.tv_sec += test_duration;
    
    long long encode_decode_count = 0;
    long long encode_decode_bytes = 0;
    last_sec = -1;
    last_count = 0;
    last_bytes = 0;
    last_print_time = start_time;
    
    while (1)
    {
        struct timespec current_time;
        clock_gettime(CLOCK_MONOTONIC, &current_time);
        
        int elapsed_sec = current_time.tv_sec - start_time.tv_sec;
        if (elapsed_sec > last_sec && elapsed_sec > 0 && elapsed_sec <= test_duration)
        {
            double elapsed = (current_time.tv_sec - last_print_time.tv_sec) + 
                           (current_time.tv_nsec - last_print_time.tv_nsec) / 1000000000.0;
            long long count_diff = encode_decode_count - last_count;
            long long bytes_diff = encode_decode_bytes - last_bytes;
            double msgs_per_sec = count_diff / elapsed;
            double mb_per_sec = (bytes_diff / 1024.0 / 1024.0) / elapsed;
            
            printf("  [%3d sec] Total: %12lld msgs, %12lld bytes | Speed: %10.2f msgs/sec, %8.2f MB/sec\n", 
                   elapsed_sec, encode_decode_count, encode_decode_bytes, msgs_per_sec, mb_per_sec);
            
            last_sec = elapsed_sec;
            last_count = encode_decode_count;
            last_bytes = encode_decode_bytes;
            last_print_time = current_time;
        }
        
        if (current_time.tv_sec > end_time.tv_sec || 
            (current_time.tv_sec == end_time.tv_sec && current_time.tv_nsec >= end_time.tv_nsec))
        {
            break;
        }
        
        dst_len = ez_hdlc_make_packet(dest, sizeof(dest), src, src_len, EZ_HDLC_MODE_VERIFY);
        if (dst_len > 0)
        {
            data_len = ez_hdlc_get_data(decoded, sizeof(decoded), dest, dst_len, EZ_HDLC_MODE_VERIFY);
            if (data_len > 0)
            {
                encode_decode_count++;
                encode_decode_bytes += dst_len;
            }
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double encode_decode_time = (end_time.tv_sec - start_time.tv_sec) + 
                                (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0;
    double encode_decode_per_sec = encode_decode_count / encode_decode_time;
    double encode_decode_mb_per_sec = (encode_decode_bytes / 1024.0 / 1024.0) / encode_decode_time;
    printf("  Encode+Decode Result: %lld msgs, %lld bytes, %.2f sec | Avg: %.2f msgs/sec, %.2f MB/sec\n\n", 
           encode_decode_count, encode_decode_bytes, encode_decode_time, encode_decode_per_sec, encode_decode_mb_per_sec);

    hdlc_delete(p_handler);

    return 0;
}
