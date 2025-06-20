// epoll版聊天室服务器
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#define PORT 8888
#define MAX_CLIENTS 10
#define BUF_SIZE 1024
#define MAX_EVENTS 16

// 客户端fd列表
int client_fds[MAX_CLIENTS] = {0};

void add_client(int fd) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_fds[i] == 0) {
            client_fds[i] = fd;
            return;
        }
    }
}

void remove_client(int fd) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_fds[i] == fd) {
            client_fds[i] = 0;
            return;
        }
    }
}

void broadcast_message(int sender_fd, const char* buffer, int len) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_fds[i] != 0 && client_fds[i] != sender_fd) {
            send(client_fds[i], buffer, len, 0);
        }
    }
}

int main() {
    int server_fd, client_fd, epfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    char buffer[BUF_SIZE];
    struct epoll_event ev, events[MAX_EVENTS];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 3);
    printf("[epoll] Server listening on port %d...\n", PORT);

    epfd = epoll_create1(0);
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd, &ev);

    while (1) {
        int n = epoll_wait(epfd, events, MAX_EVENTS, -1);
        for (int i = 0; i < n; i++) {
            if (events[i].data.fd == server_fd) {
                client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen);
                printf("New connection: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                
                // 设置为非阻塞
                int flags = fcntl(client_fd, F_GETFL, 0);
                fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

                ev.events = EPOLLIN | EPOLLET; // 使用边缘触发
                ev.data.fd = client_fd;
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
                    perror("epoll_ctl: add client_fd");
                    close(client_fd);
                } else {
                    add_client(client_fd);
                }
            } else {
                int fd = events[i].data.fd;
                while (1) { // 边缘触发需要循环读取
                    int valread = read(fd, buffer, BUF_SIZE);
                    if (valread == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            // 数据已读完
                            break;
                        }
                        perror("read");
                        break;
                    }

                    if (valread <= 0) { // 客户端断开或出错
                        getpeername(fd, (struct sockaddr*)&client_addr, &addrlen);
                        printf("Client disconnected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                        close(fd);
                        remove_client(fd);
                        break;
                    } else {
                        // 广播给所有其他客户端
                        broadcast_message(fd, buffer, valread);
                    }
                }
            }
        }
    }
    return 0;
} 