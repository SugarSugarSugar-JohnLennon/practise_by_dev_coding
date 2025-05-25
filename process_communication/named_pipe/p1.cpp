#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>

// 发送方
int main() {
    const char* fifoPath = "myfifo"; // 定义管道

    // 创建命名管道
    mkfifo(fifoPath, 0666);
    //这是在创建 命名管道（FIFO） 时指定的权限，
    //0666 是一个八进制数，表示这个文件的访问权限。
    // 0666 = 0o666 = rw-rw-rw-
    // 参考 chmod 777   , +rwx

    // 打开FIFO进行写入
    int fd = open(fifoPath, O_WRONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    const char* message = "p1 send to p2 message !";
    write(fd, message, strlen(message));
    std::cout << "p1 sent: " << message << std::endl;

    close(fd);
    return 0;
}
