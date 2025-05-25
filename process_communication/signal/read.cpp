#include <iostream>
#include <csignal>
#include <unistd.h>

void signalHandler(int signum) {
    std::cout << "Received signal: " << signum << std::endl;
}

int main() {
    // 注册信号处理函数
    signal(SIGUSR1, signalHandler);

    std::cout << "Receiver PID: " << getpid() << std::endl;
    std::cout << "Waiting for signal..." << std::endl;

    // 无限等待信号
    while (true) {
        pause();  // 挂起进程，直到接收到信号
    }

    return 0;
}
