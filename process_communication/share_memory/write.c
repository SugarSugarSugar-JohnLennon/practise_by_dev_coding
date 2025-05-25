// 写进程：将数据写入共享内存
#include <sys/ipc.h>    // IPC 用于 ftok、shmget 等函数
#include <sys/shm.h>    // 共享内存相关函数和常量
#include <stdio.h>     

int main()
{
    // 1. 使用 ftok 生成唯一 key（依赖于 "shmfile" 文件和项目 ID 65）
    // 要求 "shmfile" 存在，可以用命令行 touch shmfile 创建
    key_t key = ftok("shmfile", 65);

    // 2. 创建共享内存段或获取已有共享内存段
    // 参数说明：
    // - key：唯一标识符
    // - 1024：共享内存大小（字节）
    // - 0666 | IPC_CREAT：权限（读写）+ 不存在则创建
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    // 3. 将共享内存附加到当前进程的地址空间
    // 返回指向共享内存首地址的指针
    char *str = (char*) shmat(shmid, (void*)0, 0);

    // 4. 向共享内存写入数据（从用户输入）
    // gets 不安全，可能导致缓冲区溢出，建议使用 fgets
    // gets(str);
    fgets(str, 1024, stdin);

    // 5. 显示写入的数据
    printf("Data written in memory: %s\n", str);

    // 6. 脱离共享内存（不等于删除）
    shmdt(str);

    return 0;
}
