// 写进程
#include <stdio.h>        
#include <sys/ipc.h>      
#include <sys/msg.h>      

// 定义消息结构体
struct mesg_buffer {
    long mesg_type;           // 消息类型，必须 > 0，用于系统内部识别
    char mesg_text[100];      // 消息正文（最多99字符 + '\0'）
} message;

int main()
{
    key_t key;    // 用于生成唯一 key 的变量
    int msgid;    // 消息队列标识符

    // 使用 ftok 生成一个唯一的 key（依赖文件名和项目ID）
    // "progfile" 是参考文件（必须存在），65 是项目ID
    key = ftok("progfile", 65);

    // 创建消息队列，权限为 0666（所有人可读写），如果存在则直接返回 msgid
    msgid = msgget(key, 0666 | IPC_CREAT);

    // 设置消息类型
    message.mesg_type = 1;

    // 从用户获取输入的消息正文
    printf("Write Data : ");
    //gets(message.mesg_text);  // ⚠️ 注意：gets 是不安全的，可能导致缓冲区溢出，推荐使用 fgets() 替代
    fgets(message.mesg_text, sizeof(message.mesg_text), stdin);
    // 发送消息到队列
    // 注意：msgsnd 的第三个参数是消息正文长度（不包含 type）
    msgsnd(msgid, &message, sizeof(message.mesg_text), 0);

    // 显示已发送的消息
    printf("Data send is : %s \n", message.mesg_text);

    return 0;
}
