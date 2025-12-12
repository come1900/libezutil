/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * hdlc_tcp_client.c - HDLC over TCP Client
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: hdlc_tcp_client.c 5884 2016-12-15 09:29:31Z WuJunjie $
 *
 *  Explain:
 *     -
 *       HDLC over TCP Client with auto-reconnect
 *       Uses epoll and timerfd for event-driven I/O and reconnect timing
 *     -
 *
 *  Update:
 *     2016-12-15 09:29:31  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>

#include <ezutil/ez_hdlc.h>

#define MAX_EVENTS 16
#define BUFFER_SIZE 4096
#define DEFAULT_PORT 8888
#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_RECONNECT_INTERVAL 5  // seconds
#define DEFAULT_MAX_RECONNECT -1  // -1 means unlimited

typedef struct {
    char *host;
    int port;
    bool auto_reconnect;
    int max_reconnect;  // -1 means unlimited
    int reconnect_interval;  // seconds
} client_config_t;

typedef struct {
    int fd;
    int timer_fd;
    int epoll_fd;
    HDLC_HANDLER_T *hdlc_handler;
    client_config_t config;
    int reconnect_count;
    bool connected;
    bool should_reconnect;
} client_t;

static client_t g_client;
static volatile bool g_running = true;

// HDLC callback for received data
int hdlc_client_callback(unsigned char *buf, int len, void *arg)
{
    (void)arg;
    
    // Extract data from HDLC packet
    unsigned char data[BUFFER_SIZE];
    int data_len = ez_hdlc_get_data(data, sizeof(data), buf, len, EZ_HDLC_MODE_VERIFY);
    
    if (data_len > 0)
    {
        printf("Received: ");
        for (int i = 0; i < data_len; i++)
        {
            printf("%c", (data[i] >= 32 && data[i] < 127) ? data[i] : '.');
        }
        printf(" (%d bytes)\n", data_len);
    }
    
    return 0;
}

static void signal_handler(int sig)
{
    (void)sig;
    g_running = false;
}

static int set_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0)
    {
        return -1;
    }
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

static int create_timer_fd(int interval_sec)
{
    int fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    if (fd < 0)
    {
        perror("timerfd_create");
        return -1;
    }
    
    struct itimerspec its;
    its.it_value.tv_sec = interval_sec;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = interval_sec;
    its.it_interval.tv_nsec = 0;
    
    if (timerfd_settime(fd, 0, &its, NULL) < 0)
    {
        perror("timerfd_settime");
        close(fd);
        return -1;
    }
    
    return fd;
}

static int connect_to_server(const char *host, int port)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        perror("socket");
        return -1;
    }
    
    if (set_nonblocking(fd) < 0)
    {
        close(fd);
        return -1;
    }
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, host, &addr.sin_addr) <= 0)
    {
        fprintf(stderr, "Invalid address: %s\n", host);
        close(fd);
        return -1;
    }
    
    int ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0 && errno != EINPROGRESS)
    {
        perror("connect");
        close(fd);
        return -1;
    }
    
    // Check connection status using epoll
    struct epoll_event ev;
    ev.events = EPOLLOUT | EPOLLET;
    ev.data.fd = fd;
    
    if (epoll_ctl(g_client.epoll_fd, EPOLL_CTL_ADD, fd, &ev) < 0)
    {
        perror("epoll_ctl");
        close(fd);
        return -1;
    }
    
    // Wait for connection
    struct epoll_event events[1];
    int nfds = epoll_wait(g_client.epoll_fd, events, 1, 1000);
    if (nfds > 0 && events[0].data.fd == fd)
    {
        int error = 0;
        socklen_t len = sizeof(error);
        if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len) < 0 || error != 0)
        {
            if (error != 0)
            {
                errno = error;
                perror("connect");
            }
            epoll_ctl(g_client.epoll_fd, EPOLL_CTL_DEL, fd, NULL);
            close(fd);
            return -1;
        }
    }
    else
    {
        epoll_ctl(g_client.epoll_fd, EPOLL_CTL_DEL, fd, NULL);
        close(fd);
        return -1;
    }
    
    // Change to EPOLLIN for data
    ev.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(g_client.epoll_fd, EPOLL_CTL_MOD, fd, &ev) < 0)
    {
        perror("epoll_ctl");
        close(fd);
        return -1;
    }
    
    return fd;
}

static int init_client(client_config_t *config)
{
    memset(&g_client, 0, sizeof(g_client));
    g_client.config = *config;
    g_client.reconnect_count = 0;
    g_client.connected = false;
    g_client.should_reconnect = config->auto_reconnect;
    
    // Create epoll
    g_client.epoll_fd = epoll_create1(0);
    if (g_client.epoll_fd < 0)
    {
        perror("epoll_create1");
        return -1;
    }
    
    // Create HDLC handler
    g_client.hdlc_handler = hdlc_new();
    if (g_client.hdlc_handler == NULL)
    {
        close(g_client.epoll_fd);
        return -1;
    }
    g_client.hdlc_handler->pHdlcCallback = hdlc_client_callback;
    
    // Create reconnect timer if auto_reconnect is enabled
    if (g_client.should_reconnect)
    {
        g_client.timer_fd = create_timer_fd(config->reconnect_interval);
        if (g_client.timer_fd < 0)
        {
            hdlc_delete(g_client.hdlc_handler);
            close(g_client.epoll_fd);
            return -1;
        }
        
        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = g_client.timer_fd;
        if (epoll_ctl(g_client.epoll_fd, EPOLL_CTL_ADD, g_client.timer_fd, &ev) < 0)
        {
            perror("epoll_ctl");
            close(g_client.timer_fd);
            hdlc_delete(g_client.hdlc_handler);
            close(g_client.epoll_fd);
            return -1;
        }
    }
    else
    {
        g_client.timer_fd = -1;
    }
    
    return 0;
}

static void cleanup_client(void)
{
    if (g_client.fd >= 0)
    {
        close(g_client.fd);
        g_client.fd = -1;
    }
    if (g_client.timer_fd >= 0)
    {
        close(g_client.timer_fd);
        g_client.timer_fd = -1;
    }
    if (g_client.hdlc_handler)
    {
        hdlc_delete(g_client.hdlc_handler);
        g_client.hdlc_handler = NULL;
    }
    if (g_client.epoll_fd >= 0)
    {
        close(g_client.epoll_fd);
        g_client.epoll_fd = -1;
    }
}

static int try_connect(void)
{
    if (g_client.connected)
    {
        return 0;
    }
    
    printf("Connecting to %s:%d...\n", g_client.config.host, g_client.config.port);
    
    int fd = connect_to_server(g_client.config.host, g_client.config.port);
    if (fd < 0)
    {
        g_client.reconnect_count++;
        if (g_client.config.max_reconnect >= 0 && 
            g_client.reconnect_count > g_client.config.max_reconnect)
        {
            printf("Max reconnect attempts (%d) reached. Giving up.\n", 
                   g_client.config.max_reconnect);
            g_client.should_reconnect = false;
            return -1;
        }
        
        if (g_client.should_reconnect)
        {
            printf("Connection failed. Will retry in %d seconds... (attempt %d)\n",
                   g_client.config.reconnect_interval, g_client.reconnect_count);
        }
        return -1;
    }
    
    g_client.fd = fd;
    g_client.connected = true;
    g_client.reconnect_count = 0;
    
    printf("Connected to %s:%d\n", g_client.config.host, g_client.config.port);
    
    // Add socket to epoll
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = fd;
    if (epoll_ctl(g_client.epoll_fd, EPOLL_CTL_ADD, fd, &ev) < 0)
    {
        perror("epoll_ctl");
        close(fd);
        g_client.fd = -1;
        g_client.connected = false;
        return -1;
    }
    
    return 0;
}

static void handle_disconnect(void)
{
    if (g_client.fd >= 0)
    {
        epoll_ctl(g_client.epoll_fd, EPOLL_CTL_DEL, g_client.fd, NULL);
        close(g_client.fd);
        g_client.fd = -1;
    }
    g_client.connected = false;
    printf("Disconnected from server\n");
    
    if (g_client.hdlc_handler)
    {
        // Reset HDLC handler
        hdlc_delete(g_client.hdlc_handler);
        g_client.hdlc_handler = hdlc_new();
        if (g_client.hdlc_handler)
        {
            g_client.hdlc_handler->pHdlcCallback = hdlc_client_callback;
        }
    }
}

static void handle_client_data(void)
{
    unsigned char buffer[BUFFER_SIZE];
    
    while (1)
    {
        ssize_t n = recv(g_client.fd, buffer, sizeof(buffer), 0);
        if (n < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;  // No more data
            }
            perror("recv");
            handle_disconnect();
            break;
        }
        else if (n == 0)
        {
            // Connection closed
            handle_disconnect();
            break;
        }
        else
        {
            // Process HDLC data
            int ret = hdlc_ondata(g_client.hdlc_handler, buffer, n, NULL);
            if (ret < 0 && ret != -2)  // -2 is buffer overflow, can be ignored for now
            {
                printf("hdlc_ondata error: %d\n", ret);
            }
        }
    }
}

static void handle_timer(void)
{
    uint64_t expirations;
    ssize_t s = read(g_client.timer_fd, &expirations, sizeof(expirations));
    if (s != sizeof(expirations))
    {
        return;
    }
    
    if (!g_client.connected && g_client.should_reconnect)
    {
        try_connect();
    }
}

static int send_hdlc_data(const unsigned char *data, int data_len)
{
    if (!g_client.connected || g_client.fd < 0)
    {
        printf("Not connected\n");
        return -1;
    }
    
    unsigned char packet[BUFFER_SIZE] = {0};
    int packet_len = ez_hdlc_make_packet(packet, sizeof(packet), (unsigned char *)data, data_len, EZ_HDLC_MODE_VERIFY);
    if (packet_len <= 0)
    {
        printf("Failed to make HDLC packet: %d\n", packet_len);
        return -1;
    }
    
    ssize_t sent = send(g_client.fd, packet, packet_len, 0);
    if (sent < 0)
    {
        perror("send");
        handle_disconnect();
        return -1;
    }
    
    return (int)sent;
}

static int run_client(void)
{
    // Initial connection
    if (try_connect() < 0 && !g_client.should_reconnect)
    {
        return -1;
    }
    
    struct epoll_event events[MAX_EVENTS];
    
    while (g_running)
    {
        int nfds = epoll_wait(g_client.epoll_fd, events, MAX_EVENTS, 1000);
        if (nfds < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            perror("epoll_wait");
            break;
        }
        
        for (int i = 0; i < nfds; i++)
        {
            if (events[i].data.fd == g_client.timer_fd)
            {
                handle_timer();
            }
            else if (events[i].data.fd == g_client.fd)
            {
                handle_client_data();
            }
        }
    }
    
    cleanup_client();
    return 0;
}

static void print_usage(const char *progname)
{
    printf("Usage: %s [options]\n", progname);
    printf("Options:\n");
    printf("  -h <host>     Server host (default: %s)\n", DEFAULT_HOST);
    printf("  -p <port>     Server port (default: %d)\n", DEFAULT_PORT);
    printf("  -r            Enable auto-reconnect (default: disabled)\n");
    printf("  -n <count>    Max reconnect attempts (-1 for unlimited, default: %d)\n", DEFAULT_MAX_RECONNECT);
    printf("  -i <seconds>  Reconnect interval in seconds (default: %d)\n", DEFAULT_RECONNECT_INTERVAL);
    printf("  -s <data>     Send data to server\n");
    printf("\n");
    printf("Examples:\n");
    printf("  %s -h 127.0.0.1 -p 8888 -r -n 5 -i 3\n", progname);
    printf("  %s -h 192.168.1.100 -p 8888 -r -n -1 -s \"Hello\"\n", progname);
}

int main(int argc, char *argv[])
{
    client_config_t config = {
        .host = DEFAULT_HOST,
        .port = DEFAULT_PORT,
        .auto_reconnect = false,
        .max_reconnect = DEFAULT_MAX_RECONNECT,
        .reconnect_interval = DEFAULT_RECONNECT_INTERVAL
    };
    
    char *send_data = NULL;
    int opt;
    
    while ((opt = getopt(argc, argv, "h:p:rn:i:s:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                config.host = optarg;
                break;
            case 'p':
                config.port = atoi(optarg);
                if (config.port <= 0 || config.port > 65535)
                {
                    fprintf(stderr, "Invalid port number: %s\n", optarg);
                    return 1;
                }
                break;
            case 'r':
                config.auto_reconnect = true;
                break;
            case 'n':
                config.max_reconnect = atoi(optarg);
                break;
            case 'i':
                config.reconnect_interval = atoi(optarg);
                if (config.reconnect_interval <= 0)
                {
                    fprintf(stderr, "Invalid reconnect interval: %s\n", optarg);
                    return 1;
                }
                break;
            case 's':
                send_data = optarg;
                break;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }
    
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    printf("HDLC TCP Client Configuration:\n");
    printf("  Host: %s\n", config.host);
    printf("  Port: %d\n", config.port);
    printf("  Auto-reconnect: %s\n", config.auto_reconnect ? "enabled" : "disabled");
    if (config.auto_reconnect)
    {
    if (config.max_reconnect < 0)
    {
        printf("  Max reconnect: unlimited\n");
    }
    else
    {
        printf("  Max reconnect: %d\n", config.max_reconnect);
    }
        printf("  Reconnect interval: %d seconds\n", config.reconnect_interval);
    }
    printf("\n");
    
    if (init_client(&config) < 0)
    {
        fprintf(stderr, "Failed to initialize client\n");
        return 1;
    }
    
    // Send initial data if provided
    if (send_data)
    {
        // Wait a bit for connection
        usleep(100000);  // 100ms
        if (g_client.connected)
        {
            send_hdlc_data((unsigned char *)send_data, strlen(send_data));
        }
    }
    
    int ret = run_client();
    
    printf("Client stopped\n");
    return ret;
}

