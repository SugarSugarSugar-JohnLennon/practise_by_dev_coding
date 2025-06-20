// poll版聊天室客户端
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <poll.h>

#define PORT 8888
#define BUF_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    struct pollfd fds[2];

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
    printf("[poll] Connected to chat server. Type messages:\n");
    fds[0].fd = 0; fds[0].events = POLLIN;
    fds[1].fd = sockfd; fds[1].events = POLLIN;
    while (1) {
        poll(fds, 2, -1);
        if (fds[0].revents & POLLIN) {
            fgets(buffer, BUF_SIZE, stdin);
            send(sockfd, buffer, strlen(buffer), 0);
        }
        if (fds[1].revents & POLLIN) {
            int valread = read(sockfd, buffer, BUF_SIZE - 1);
            if (valread <= 0) {
                printf("Server disconnected.\n");
                break;
            }
            buffer[valread] = '\0';
            printf(">> %s", buffer);
        }
    }
    close(sockfd);
    return 0;
} 