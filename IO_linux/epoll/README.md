# epoll I/O复用学习笔记（个人学习记录）

## epoll原理简介
epoll是Linux专有的高性能I/O多路复用机制，适合大规模并发连接。epoll采用事件驱动（边缘/水平触发），内核高效管理fd集合，极大提升了高并发下的性能。

## 与select/poll的对比
- epoll无fd数量限制，且事件通知机制更高效。
- epoll支持边缘触发（ET）和水平触发（LT）。
- epoll只返回活跃fd，避免无效遍历。
- 适合高并发服务器场景。

## 代码结构
- server.c：聊天室服务器，支持多客户端，消息转发，基于epoll实现。
- client.c：聊天室客户端，支持收发消息。

## 编译与运行
```sh
# 编译
$ gcc server.c -o bin/server
$ gcc client.c -o bin/client
# 运行
$ ./bin/server
# 新开终端，运行多个客户端
$ ./bin/client
```

## 常见问题
- epoll_ctl增删fd要正确，断开连接要及时删除。
- 默认水平触发，边缘触发需额外处理。
- 事件数组大小要合理，防止丢事件。

## 个人总结
epoll是高性能网络编程的核心，理解其事件驱动模型对开发高并发服务器至关重要。建议多做实验，结合strace等工具深入理解。 