# epoll I/O多路复用学习笔记（个人学习记录）

## 1. `epoll` 原理简介
`epoll` 是 Linux 下最高性能的I/O多路复用机制，是 `select` 和 `poll` 的重大升级。它的核心优势在于，从根本上解决了前两者的性能瓶颈。`epoll` 在内核中维护一个专属的数据结构（如红黑树和链表），用于存储所有被监听的 `fd`。这带来了两大革命性改变：
1.  **无需重复拷贝**：`fd` 集合只需通过 `epoll_ctl` 操作一次，之后 `epoll_wait` 调用不再需要在用户态和内核态之间拷贝 `fd` 列表。
2.  **事件驱动**：内核只返回真正就绪的 `fd`，而不是像 `select`/`poll` 那样让用户程序自己去遍历整个列表。当 `fd` 就绪时，内核会通过回调机制将其加入一个"就绪链表"，`epoll_wait` 只需检查这个链表即可。

## 2. 核心API详解
- **`epoll_create1(int flags)`**:
  创建一个 `epoll` 实例，并返回一个指向它的文件描述符（`epfd`）。后续所有操作都通过这个 `epfd` 进行。

- **`epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)`**:
  `epoll` 的核心控制器，用于增、删、改 `epoll` 实例中监听的 `fd`。
  - `epfd`: `epoll_create1` 返回的 `epoll` 实例 `fd`。
  - `op`: 操作类型，如 `EPOLL_CTL_ADD` (添加), `EPOLL_CTL_MOD` (修改), `EPOLL_CTL_DEL` (删除)。
  - `fd`: 要操作的目标文件描述符。
  - `event`: 指向 `epoll_event` 结构的指针，描述了要监听的事件。

- **`struct epoll_event`**:
  ```c
  struct epoll_event {
      uint32_t     events;      /* Epoll events */
      epoll_data_t data;        /* User data variable */
  };
  ```
  - `events`: 事件类型，如 `EPOLLIN` (读就绪), `EPOLLOUT` (写就绪), `EPOLLET` (边缘触发)。
  - `data`: 一个联合体，通常用 `data.fd` 来存储与事件关联的 `fd`，这样 `epoll_wait` 返回时我们能直接知道是哪个 `fd` 就绪了。

- **`epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)`**:
  等待事件发生，相当于 `select` 和 `poll` 的主体功能。
  - `epfd`: `epoll` 实例 `fd`。
  - `events`: 一个 `epoll_event` 数组，用于从内核接收就绪的事件。
  - `maxevents`: `events` 数组的大小。
  - `timeout`: 超时时间（毫秒）。
  该函数返回就绪的 `fd` 数量，这些就绪的 `fd` 信息被填充到 `events` 数组中。

## 3. 工作模式：LT vs ET
- **水平触发 (Level-Triggered, LT)** - 默认模式：
  只要 `fd` 的读/写缓冲区中还有数据，`epoll_wait` 每次都会返回该 `fd`。**代码编写简单，不易出错**，逻辑类似 `poll`。
- **边缘触发 (Edge-Triggered, ET)** - 高性能模式：
  只有当 `fd` 的状态 **发生变化时**（例如，数据从无到有），`epoll_wait` 才会返回一次。**这要求程序必须一次性将缓冲区的数据读/写完**，否则剩余的数据不会再触发 `epoll_wait`。通常需要配合非阻塞I/O (`O_NONBLOCK`)，并用 `while` 循环处理读写，直到返回 `EAGAIN` 或 `EWOULDBLOCK`。

## 4. 服务端代码逻辑解析 (`server.c` - 最终ET版)
1.  **初始化**: 创建 `server_fd` 和 `epfd`。使用 `epoll_ctl` 将 `server_fd` 添加到 `epoll` 实例中，监听 `EPOLLIN` 事件。
2.  **主循环**: 调用 `epoll_wait()` 等待事件。
3.  **处理新连接**: 如果就绪的 `fd` 是 `server_fd`，则调用 `accept()`。
    -   将新 `client_fd` **设置为非阻塞模式**。
    -   创建一个 `epoll_event`，将其模式设置为 `EPOLLIN | EPOLLET` (读就绪 + 边缘触发)。
    -   使用 `epoll_ctl` 将 `client_fd` 添加到 `epoll` 实例。
    -   将 `client_fd` 添加到我们自己维护的 `client_fds` 数组中，用于后续的广播。
4.  **处理客户端消息**: 如果就绪的 `fd` 是一个客户端 `fd`：
    -   **进入一个 `while(1)` 循环**，持续调用 `read()`，直到缓冲区数据被读完。
    -   如果 `read()` 返回 `EAGAIN` 或 `EWOULDBLOCK`，说明数据已读完，退出 `while` 循环。
    -   如果 `read()` 成功，则调用 `broadcast_message` 函数将数据转发给其他所有客户端。
    -   如果 `read()` 表明连接断开，则用 `epoll_ctl` 将其从 `epoll` 实例中移除，`close(fd)`，并从 `client_fds` 数组中移除。

## 5. 编译与运行
```sh
# (确保在epoll目录下)
./build.sh
# 运行服务端
./bin/server
# (新开终端) 运行客户端
./bin/client
```

## 6. 个人总结
`epoll` 是 Linux 高性能网络编程的基石。它的设计从根本上解决了 `select`/`poll` 的性能瓶颈，使得构建百万级并发连接的服务器成为可能。虽然边缘触发模式的编程比水平触发更复杂，但它能最大化 `epoll` 的性能优势，是生产级应用的首选。通过这个项目，我深刻理解了从 `select` 到 `epoll` 的演进过程，以及事件驱动模型的设计精髓。 