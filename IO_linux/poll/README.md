# poll I/O多路复用学习笔记（个人学习记录）

## 1. `poll` 原理简介
`poll` 是对 `select` 的一种改进。它使用一个 `struct pollfd` 结构体数组来代替 `select` 的 `fd_set`。`poll` 同样需要将这个数组从用户态拷贝到内核态，由内核遍历检查事件，但它解决了 `select` 的两个主要问题：一是取消了1024个文件描述符的数量限制；二是 `fd` 和 `events` 分开管理，每次调用后无需重置整个集合。

## 2. 核心API详解
- **`struct pollfd`**:
  这是 `poll` 的核心数据结构，定义如下：
  ```c
  struct pollfd {
      int   fd;         /* file descriptor */
      short events;     /* requested events */
      short revents;    /* returned events */
  };
  ```
  - `fd`: 要监听的文件描述符。
  - `events`: 我们 **请求** 内核监听的事件类型（如 `POLLIN`, `POLLOUT`）。
  - `revents`: `poll` 调用返回后，内核 **实际** 在该 `fd` 上检测到的事件类型。

- **`poll(struct pollfd *fds, nfds_t nfds, int timeout)`**:
  这是 `poll` 的核心函数。
  - `fds`: 指向 `pollfd` 数组的指针。
  - `nfds`: `fds` 数组的长度。
  - `timeout`: 超时时间（毫秒）。`-1`表示永久阻塞，`0`表示非阻塞。
  `poll` 返回后，我们只需遍历 `fds` 数组，检查每个元素的 `revents` 字段，就能知道哪个 `fd` 上发生了什么事件。

## 3. 服务端代码逻辑解析 (`server.c`)
1.  **初始化**: 创建 `server_fd`，绑定并监听端口。初始化一个 `pollfd` 数组 `fds`，大小为 `MAX_CLIENTS + 1`。将 `fds[0]` 用于监听 `server_fd` 的新连接事件 (`POLLIN`)。
2.  **主循环 `while(1)`**:
    -   调用 `poll()` 阻塞等待事件。
    -   `poll` 返回后，开始检查事件。
3.  **处理新连接**: 检查 `fds[0].revents` 是否包含 `POLLIN`。如果是，调用 `accept()` 接受新连接，并找一个空闲的 `fds[i]`（`fd == -1`），将新客户端的 `fd` 存入 `fds[i].fd`，并设置监听 `fds[i].events = POLLIN`。
4.  **处理客户端消息**: 从 `i = 1` 开始遍历 `fds` 数组。
    -   如果 `fds[i].fd` 不为-1且其 `revents` 包含 `POLLIN`，表示该客户端发来了数据。
    -   调用 `read()` 读取数据。
        -   若 `read()` 成功，则遍历 `fds` 数组，将消息广播给所有其他客户端（`j != i` 且 `fd != -1`）。
        -   若 `read()` 表明客户端已断开，则 `close(fds[i].fd)` 并将其 `fd` 置为-1，以便复用。

## 4. 优缺点分析
- **优点**:
  - **无fd数量限制**：`poll` 管理的 `fd` 数量只受限于系统资源。
  - **API设计更合理**：将输入事件(`events`)和输出事件(`revents`)分开，无需每次重置。
- **缺点**:
  - **性能问题依然存在**：
    1.  **内存拷贝开销**：每次调用仍需在用户态和内核态之间拷贝整个 `pollfd` 数组。
    2.  **内核遍历开销**：随着连接数增多，内核需要遍历的 `fds` 数组也变大，性能呈线性下降。

## 5. 编译与运行
```sh
# (确保在poll目录下)
./build.sh
# 运行服务端
./bin/server
# (新开终端) 运行客户端
./bin/client
```

## 6. 个人总结
`poll` 是 `select` 的直接升级版，解决了 `fd` 数量限制的关键问题，API也更友好。它和 `select` 一样，在连接数不多的情况下表现良好。但其核心性能瓶颈（拷贝和遍历）没有根本解决，这为更高性能的 `epoll` 登场埋下了伏笔。 