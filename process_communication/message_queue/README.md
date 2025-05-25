# 消息队列
由于管道不适合进程间频繁地交换数据，消息队列则可以解决这个问题。A进程要给B进程发送消息，A进程把数据放到对应的消息队列之后就可以正常返回了，B进程需要的时候再去读取数据就可以。

如果没有释放消息队列或者关闭操作系统，消息队列会一直存在，而匿名管道是随进程的创建而建立，随进程的结束而销毁。

消息队列本质上是位于内核空间的链表，链表的每个节点都是一条消息。每一条消息都有自己的消息类型，消息类型用整数来表示，而且必须大于 0。每种类型的消息都被对应的链表所维护。

消息队列不适合比较大的数据的传输。

相关的函数：
```c++
// 创建和获取 IPC 内核对象
int msgget(key_t key, int flags);

// 将消息发送到消息队列
int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);

// 从消息队列获取消息
ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);

// 查看、设置、删除 IPC 内核对象（用法和 shmctl 一样）
int msgctl(int msqid, int cmd, struct msqid_ds *buf);
```

```c++
struct Msg {
    long type;  // 消息类型。这个是必须的，并且值必须 > 0，这个值被系统使用
    // 消息正文，多少字节随你而定
    // ...
};

写进程代码：[write.c](write.c)
读进程代码：[read.c](read.c)