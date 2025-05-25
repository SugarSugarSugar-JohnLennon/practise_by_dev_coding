// 读进程：接收消息队列中的数据
#include <stdio.h>         
#include <sys/ipc.h>       // IPC：消息队列用到的 key_t 类型等
#include <sys/msg.h>       // 消息队列相关函数和结构定义

// 定义消息结构体，与发送进程必须一致
struct mesg_buffer {
    long mesg_type;        // 消息类型（必须为正数）
    char mesg_text[100];   // 消息正文
} message;

int main()
{
    key_t key;     // 消息队列的 key
    int msgid;     // 消息队列的 ID

    // 使用 ftok() 生成唯一 key（需保证 "progfile" 文件存在）
    key = ftok("progfile", 65);

    // 获取（或创建）消息队列，权限为 0666（读写权限）
    msgid = msgget(key, 0666 | IPC_CREAT);

    // 从消息队列中接收 type = 1 的消息
    // 第三个参数：只读正文长度，不包括 mesg_type
    msgrcv(msgid, &message, sizeof(message.mesg_text), 1, 0);

    // 显示接收到的消息
    printf("p2 Received from p1, data is : %s\n", message.mesg_text);

    // 删除消息队列，防止资源泄漏（可选）
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
