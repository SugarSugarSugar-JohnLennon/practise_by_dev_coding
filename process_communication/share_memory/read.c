// 读进程：从共享内存中读取数据
#include <sys/ipc.h>     
#include <sys/shm.h>     
#include <stdio.h>     

int main()
{
    // 1. 使用 ftok 创建一个唯一的 key，与写进程保持一致
    // 注意："shmfile" 文件必须存在，并且和写进程用的文件名、ID 相同
    key_t key = ftok("shmfile", 65);

    // 2. 获取共享内存段标识符（shmid）
    // 参数必须与写进程中的大小和权限一致
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    // 3. 将共享内存连接到当前进程的地址空间
    char *str = (char*) shmat(shmid, (void*)0, 0);

    // 4. 从共享内存中读取并打印数据
    printf("Data read from memory: %s\n", str);

    // 5. 脱离共享内存（仅解除映射，不删除）
    shmdt(str);

    // 6. 删除共享内存段（仅一次即可，通常放在读进程中）
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
