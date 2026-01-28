/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * hdlc_tcp_server.c - HDLC over TCP Server
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: hdlc_tcp_server.c 5884 2016-12-15 09:29:31Z WuJunjie $
 *
 *  Explain:
 *     -
 *       HDLC over TCP Server with echo functionality
 *       Uses epoll for event-driven I/O
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
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>

#include <ezutil/ez_hdlc.h>

#define MAX_EVENTS 64
#define MAX_CLIENTS 1024
#define BUFFER_SIZE 4096
#define DEFAULT_PORT 8888

typedef struct {
    int fd;
    HDLC_HANDLER_T *hdlc_handler;
    bool active;
} client_conn_t;

typedef struct {
    int listen_fd;
    int epoll_fd;
    int port;
    client_conn_t clients[MAX_CLIENTS];
    int client_count;
} server_t;

static server_t g_server;
static volatile bool g_running = true;

// HDLC callback for echo
int hdlc_echo_callback(unsigned char *buf, int len, void *arg)
{
    int client_fd = (int)(long)arg;
    unsigned char *packet = buf;
    int packet_len = len;
    
    // Extract data from HDLC packet
    unsigned char data[BUFFER_SIZE];
    int data_len = ez_hdlc_get_data(data, sizeof(data), packet, packet_len, EZ_HDLC_MODE_VERIFY);
    
    if (data_len > 0)
    {
        // Echo: make HDLC packet and send back
        unsigned char echo_packet[BUFFER_SIZE];
        int echo_packet_len = ez_hdlc_make_packet(echo_packet, sizeof(echo_packet), 
                                                   data, data_len, EZ_HDLC_MODE_VERIFY);
        if (echo_packet_len > 0)
        {
            ssize_t sent = send(client_fd, echo_packet, echo_packet_len, 0);
            if (sent < 0)
            {
                perror("send");
            }
        }
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

static int create_listen_socket(int port)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        perror("socket");
        return -1;
    }
    
    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        close(fd);
        return -1;
    }
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        close(fd);
        return -1;
    }
    
    if (listen(fd, SOMAXCONN) < 0)
    {
        perror("listen");
        close(fd);
        return -1;
    }
    
    if (set_nonblocking(fd) < 0)
    {
        perror("fcntl");
        close(fd);
        return -1;
    }
    
    return fd;
}

static int add_client(int client_fd)
{
    if (g_server.client_count >= MAX_CLIENTS)
    {
        close(client_fd);
        return -1;
    }
    
    int idx = -1;
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (!g_server.clients[i].active)
        {
            idx = i;
            break;
        }
    }
    
    if (idx < 0)
    {
        close(client_fd);
        return -1;
    }
    
    client_conn_t *client = &g_server.clients[idx];
    client->fd = client_fd;
    client->hdlc_handler = hdlc_new();
    if (client->hdlc_handler == NULL)
    {
        close(client_fd);
        return -1;
    }
    client->hdlc_handler->pHdlcCallback = hdlc_echo_callback;
    client->hdlc_handler->obj = (void *)(long)client_fd;
    client->active = true;
    g_server.client_count++;
    
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.u32 = idx;  // Store client index
    
    if (epoll_ctl(g_server.epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0)
    {
        perror("epoll_ctl");
        hdlc_delete(client->hdlc_handler);
        close(client_fd);
        client->active = false;
        g_server.client_count--;
        return -1;
    }
    
    printf("Client connected: fd=%d, total=%d\n", client_fd, g_server.client_count);
    return 0;
}

static void remove_client(int idx)
{
    if (idx < 0 || idx >= MAX_CLIENTS || !g_server.clients[idx].active)
    {
        return;
    }
    
    client_conn_t *client = &g_server.clients[idx];
    
    epoll_ctl(g_server.epoll_fd, EPOLL_CTL_DEL, client->fd, NULL);
    close(client->fd);
    hdlc_delete(client->hdlc_handler);
    client->active = false;
    g_server.client_count--;
    
    printf("Client disconnected: fd=%d, total=%d\n", client->fd, g_server.client_count);
}

static void handle_client_data(int idx)
{
    if (idx < 0 || idx >= MAX_CLIENTS || !g_server.clients[idx].active)
    {
        return;
    }
    
    client_conn_t *client = &g_server.clients[idx];
    unsigned char buffer[BUFFER_SIZE];
    
    while (1)
    {
        ssize_t n = recv(client->fd, buffer, sizeof(buffer), 0);
        if (n < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;  // No more data
            }
            perror("recv");
            remove_client(idx);
            break;
        }
        else if (n == 0)
        {
            // Connection closed
            remove_client(idx);
            break;
        }
        else
        {
            // Process HDLC data
            int ret = hdlc_ondata(client->hdlc_handler, buffer, n, (void *)(long)client->fd);
            if (ret < 0 && ret != -2)  // -2 is buffer overflow, can be ignored for now
            {
                printf("hdlc_ondata error: %d\n", ret);
            }
        }
    }
}

static int run_server(int port)
{
    g_server.port = port;
    g_server.client_count = 0;
    memset(g_server.clients, 0, sizeof(g_server.clients));
    
    // Create listen socket
    g_server.listen_fd = create_listen_socket(port);
    if (g_server.listen_fd < 0)
    {
        return -1;
    }
    
    // Create epoll
    g_server.epoll_fd = epoll_create1(0);
    if (g_server.epoll_fd < 0)
    {
        perror("epoll_create1");
        close(g_server.listen_fd);
        return -1;
    }
    
    // Add listen socket to epoll
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.u32 = MAX_CLIENTS;  // Special marker for listen socket
    
    if (epoll_ctl(g_server.epoll_fd, EPOLL_CTL_ADD, g_server.listen_fd, &ev) < 0)
    {
        perror("epoll_ctl");
        close(g_server.epoll_fd);
        close(g_server.listen_fd);
        return -1;
    }
    
    printf("HDLC TCP Server listening on port %d\n", port);
    
    struct epoll_event events[MAX_EVENTS];
    
    while (g_running)
    {
        int nfds = epoll_wait(g_server.epoll_fd, events, MAX_EVENTS, 1000);
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
            if (events[i].data.u32 == MAX_CLIENTS)
            {
                // New connection
                while (1)
                {
                    struct sockaddr_in client_addr;
                    socklen_t addr_len = sizeof(client_addr);
                    int client_fd = accept(g_server.listen_fd, 
                                           (struct sockaddr *)&client_addr, 
                                           &addr_len);
                    if (client_fd < 0)
                    {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                        {
                            break;
                        }
                        perror("accept");
                        break;
                    }
                    
                    if (set_nonblocking(client_fd) < 0)
                    {
                        close(client_fd);
                        continue;
                    }
                    
                    add_client(client_fd);
                }
            }
            else
            {
                // Client data
                handle_client_data(events[i].data.u32);
            }
        }
    }
    
    // Cleanup
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (g_server.clients[i].active)
        {
            remove_client(i);
        }
    }
    
    close(g_server.epoll_fd);
    close(g_server.listen_fd);
    
    return 0;
}

int main(int argc, char *argv[])
{
    int port = DEFAULT_PORT;
    
    if (argc > 1)
    {
        port = atoi(argv[1]);
        if (port <= 0 || port > 65535)
        {
            fprintf(stderr, "Invalid port number: %d\n", port);
            return 1;
        }
    }
    
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    printf("Starting HDLC TCP Server on port %d...\n", port);
    printf("Press Ctrl+C to stop\n");
    
    int ret = run_server(port);
    
    printf("Server stopped\n");
    return ret;
}
