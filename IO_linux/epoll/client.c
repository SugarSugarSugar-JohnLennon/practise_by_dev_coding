// epoll版聊天室客户端
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define PORT 8888
#define BUF_SIZE 1024
#define MAX_EVENTS 2

int main() {
    int sockfd, epfd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    struct epoll_event ev, events[MAX_EVENTS];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }
    printf("[epoll] Connected to chat server. Type messages:\n");
    epfd = epoll_create1(0);
    ev.events = EPOLLIN;
    ev.data.fd = 0; // stdin
    epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &ev);
    ev.data.fd = sockfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
    while (1) {
        int n = epoll_wait(epfd, events, MAX_EVENTS, -1);
        for (int i = 0; i < n; i++) {
            if (events[i].data.fd == 0) {
                fgets(buffer, BUF_SIZE, stdin);
                send(sockfd, buffer, strlen(buffer), 0);
            } else if (events[i].data.fd == sockfd) {
                int valread = read(sockfd, buffer, BUF_SIZE - 1);
                if (valread <= 0) {
                    printf("Server disconnected.\n");
                    break;
                }
                buffer[valread] = '\0';
                printf(">> %s", buffer);
            }
        }
    }
    close(sockfd);
    return 0;
} 