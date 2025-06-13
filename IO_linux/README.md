# I/O框架
---
I/O复用（Input/Output Multiplexing）是一种在单个线程或进程中同时处理多个I/O操作的技术，主要用于网络编程或需要同时处理多个输入输出流的场景。
---
## 基本概念
传统阻塞I/O模型中，每个I/O操作（如读取socket数据）会阻塞当前线程，直到数据就绪。而I/O复用技术允许你用一个线程同时监听多个I/O事件，只有当某个I/O事件准备就绪时，程序才去处理它。
---
## 常见的I/O复用机制（以Linux为例）：
- select	最早的I/O复用接口，支持的最大文件描述符数量有限（通常为1024）
- poll	类似于select，但没有描述符数量限制
- epoll	Linux专有，更高效，支持“事件驱动”模式，适合大量并发连接场景
---
## 工作流程（以 epoll 为例）：
1. 创建一个 epoll 实例。
2. 注册你感兴趣的多个文件描述符（如 socket）及其事件类型（读、写等）。
3. 调用 epoll_wait 等待事件触发。
4. 当某个描述符准备好时返回，你再去处理它。
---
## 优势
1. 避免每个连接一个线程，节省资源。
2. 可扩展性强，适合高并发服务（如Web服务器、聊天服务器）。
3. 与非阻塞I/O结合使用效率更高。
```cpp
int epfd = epoll_create();
epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd1, event1);
epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd2, event2);

while (1) {
    int n = epoll_wait(epfd, events, MAX_EVENTS, -1);
    for (int i = 0; i < n; i++) {
        handle_event(events[i].data.fd);
    }
}
```