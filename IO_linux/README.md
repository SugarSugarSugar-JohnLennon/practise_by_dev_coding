# I/O复用框架学习记录（select/poll/epoll）

---

## 目录结构

- select/  —— select多路复用聊天室示例
- poll/    —— poll多路复用聊天室示例
- epoll/   —— epoll多路复用聊天室示例

每个子目录均包含：
- server.c  多客户端聊天室服务器
- client.c  聊天室客户端
- CMakeLists.txt  CMake构建配置
- build.sh  一键构建脚本
- README.md  个人学习笔记

---

## 功能简介

- 支持多客户端聊天室，消息互转，服务端可并发处理多个连接。
- select、poll、epoll三种I/O复用方式均有完整实现，便于对比学习。
- 代码结构统一，便于迁移和扩展。

---

## 编译与运行

以epoll为例：
```sh
cd epoll
bash build.sh
./bin/server
# 新开终端，运行多个客户端
./bin/client
```
select和poll用法完全一致。

---

## 学习建议

- 建议先阅读select、poll、epoll各自README.md，理解原理和代码结构。
- 逐步对比三种实现，体会API差异和性能提升。
- 可自行扩展为群聊、私聊、文件传输等功能，加深理解。

---

## 个人总结

I/O复用是高性能网络编程的基础。通过本项目的多实现对比，能系统掌握select、poll、epoll的用法和适用场景，为后续深入学习Reactor、Proactor等模型打下基础。