#include <iostream>
#include <csignal>
#include <unistd.h>

int main() {
    pid_t targetPid;

    std::cout << "Enter receiver PID: ";
    std::cin >> targetPid;

    // 发送 SIGUSR1 信号给接收进程
    kill(targetPid, SIGUSR1);

    std::cout << "Signal SIGUSR1 sent to PID " << targetPid << std::endl;

    return 0;
}
