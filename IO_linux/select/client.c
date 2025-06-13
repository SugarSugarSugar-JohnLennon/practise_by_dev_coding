// client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 8888
#define BUF_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    fd_set readfds;

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

    printf("Connected to chat server. Type messages:\n");

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(0, &readfds);        // stdin
        FD_SET(sockfd, &readfds);   // socket
        int max_fd = sockfd;

        select(max_fd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(0, &readfds)) {
            fgets(buffer, BUF_SIZE, stdin);
            send(sockfd, buffer, strlen(buffer), 0);
        }

        if (FD_ISSET(sockfd, &readfds)) {
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
