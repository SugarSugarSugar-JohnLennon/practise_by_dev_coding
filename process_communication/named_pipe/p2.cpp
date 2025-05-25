// p2.cpp
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

// 接受方
int main() {
    const char* fifoPath = "myfifo";
    mkfifo(fifoPath, 0666);

    // 打开FIFO进行读取
    int fd = open(fifoPath, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    char buffer[100];
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        std::cout << "p2 received: " << buffer << std::endl;
    }

    close(fd);
    return 0;
}
