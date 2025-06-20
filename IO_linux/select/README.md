# select I/O多路复用学习笔记（个人学习记录）

## 1. `select` 原理简介
`select` 是*nix系统下最古老的I/O多路复用机制。它的核心思想是，将需要监听的文件描述符（fd）集合从用户态拷贝到内核态，由内核负责遍历检查这些fd是否就绪。当有fd就绪或超时后，`select`调用返回，内核再将更新后的fd集合拷贝回用户态，由程序进行处理。

## 2. 核心API详解
- **`fd_set`**:
  一个文件描述符集合，通常是一个位图（bitmap），每一位对应一个fd。其大小由`FD_SETSIZE`宏定义，通常为1024。

- **`FD_ZERO(fd_set *set)`**:
  清空一个`fd_set`，即所有位都置0。这是每次循环调用`select`前的必要操作。

- **`FD_SET(int fd, fd_set *set)`**:
  将一个`fd`添加到`fd_set`中，即将对应位置1。

- **`FD_CLR(int fd, fd_set *set)`**:
  将一个`fd`从`fd_set`中移除。

- **`FD_ISSET(int fd, fd_set *set)`**:
  检查一个`fd`是否仍在`fd_set`中（即是否就绪）。`select`返回后，我们就用这个宏来判断哪个fd上发生了事件。

- **`select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)`**:
  这是核心函数。
  - `nfds`: 需要监听的fd中最大值加1。
  - `readfds`: 指向`fd_set`的指针，用于监听读事件。
  - `writefds`, `exceptfds`: 分别用于监听写事件和异常事件。
  - `timeout`: 设置超时时间。`NULL`表示永久阻塞；时间为0表示非阻塞；有具体值表示超时时长。
  **注意**：`select`返回后，`readfds`等集合内容会被内核修改，只保留就绪的fd。因此，每次循环开始时都必须用一个备份的`fd_set`来重新初始化它。

## 3. 服务端代码逻辑解析 (`server.c`)
1.  **初始化**: 创建`server_fd`，绑定端口`8888`并开始监听。维护一个`client_sockets`数组来保存所有客户端的`fd`。
2.  **主循环 `while(1)`**:
    -   使用`FD_ZERO`清空`readfds`。
    -   使用`FD_SET`将`server_fd`（用于接受新连接）和所有已连接的`client_sockets[i]`添加到`readfds`中。
    -   找出所有fd中的最大值`max_fd`，用于`select`的第一个参数。
    -   调用`select()`阻塞等待事件。
3.  **处理新连接**: `select`返回后，用`FD_ISSET(server_fd, &readfds)`检查是否有新连接。如果有，调用`accept()`接受连接，并将新的`client_fd`存入`client_sockets`数组。
4.  **处理客户端消息**: 遍历`client_sockets`数组，用`FD_ISSET`检查每个客户端`fd`。如果就绪，调用`read()`读取数据。
    -   如果`read()`返回大于0，表示收到数据，则遍历`client_sockets`将消息广播给其他所有客户端。
    -   如果`read()`返回0或-1，表示客户端断开连接。此时需`close(sd)`并将其从`client_sockets`中移除（置为0）。

## 4. 优缺点分析
- **优点**:
  - **可移植性好**：遵循POSIX标准，几乎所有平台都支持。
  - **API简单**：易于理解和使用。
- **缺点**:
  - **性能瓶颈**：
    1.  **数量限制**：受`FD_SETSIZE`（1024）限制，不适合高并发。
    2.  **内存拷贝开销**：每次调用都需要在用户态和内核态之间拷贝整个`fd_set`。
    3.  **内核遍历开销**：内核需要线性遍历所有被监听的fd，无论它们是否活跃。

## 5. 编译与运行
```sh
# (确保在select目录下)
# 添加执行权限
chmod +x build.sh
# 一键编译
./build.sh
# 运行服务端
./bin/server
# (新开终端) 运行客户端
./bin/client
```

## 6. 个人总结
`select`是理解I/O多路复用概念的绝佳起点。虽然有性能瓶颈，但其API直观，原理简单，是学习网络编程绕不开的一环。它的设计缺陷也直接催生了`poll`和`epoll`的出现。 