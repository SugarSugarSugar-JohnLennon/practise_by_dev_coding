// poll版聊天室服务器
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

#define PORT 8888
#define MAX_CLIENTS 10
#define BUF_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    char buffer[BUF_SIZE];
    struct pollfd fds[MAX_CLIENTS + 1];
    int nfds = 1;

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
    printf("[poll] Server listening on port %d...\n", PORT);

    fds[0].fd = server_fd;
    fds[0].events = POLLIN;
    for (int i = 1; i <= MAX_CLIENTS; i++) fds[i].fd = -1;

    while (1) {
        int activity = poll(fds, nfds, -1);
        if (activity < 0) {
            perror("poll error");
            exit(EXIT_FAILURE);
        }
        // 新连接
        if (fds[0].revents & POLLIN) {
            client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen);
            printf("New connection: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            int added = 0;
            for (int i = 1; i <= MAX_CLIENTS; i++) {
                if (fds[i].fd == -1) {
                    fds[i].fd = client_fd;
                    fds[i].events = POLLIN;
                    if (i >= nfds) nfds = i + 1;
                    added = 1;
                    break;
                }
            }
            if (!added) close(client_fd);
        }
        // 客户端消息
        for (int i = 1; i < nfds; i++) {
            if (fds[i].fd != -1 && (fds[i].revents & POLLIN)) {
                int valread = read(fds[i].fd, buffer, BUF_SIZE);
                if (valread <= 0) {
                    getpeername(fds[i].fd, (struct sockaddr*)&client_addr, &addrlen);
                    printf("Client disconnected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                    close(fds[i].fd);
                    fds[i].fd = -1;
                } else {
                    buffer[valread] = '\0';
                    for (int j = 1; j < nfds; j++) {
                        if (fds[j].fd != -1 && j != i) {
                            send(fds[j].fd, buffer, strlen(buffer), 0);
                        }
                    }
                }
            }
        }
    }
    return 0;
} 