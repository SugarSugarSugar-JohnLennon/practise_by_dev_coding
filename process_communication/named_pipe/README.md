# 命名管道
- 使用 Linux 的 mkfifo() 创建一个命名管道文件（比如 /tmp/myfifo）。

- 一个程序（p1）向 FIFO 写入数据。

- 另一个程序（p2）从 FIFO 读取数据。

- 两端通过这个特殊文件进行通信。

命名管道可以在没有亲情的进程间通信。

打开两个终端，分别./bin/p2 ./bin/p1
