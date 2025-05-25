#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    // 1. 获取 key
    key_t key = ftok("semfile", 75);

    // 2. 获取共享内存
    int shmid = shmget(key, 1024, 0666);
    char* data = (char*) shmat(shmid, NULL, 0);

    // 3. 获取信号量
    int semid = semget(key, 1, 0666);

    // 4. P操作（等待写入完成）
    struct sembuf p = {0, -1, 0};
    semop(semid, &p, 1);

    // 5. 读取数据
    printf("Reader: Data read from memory: %s\n", data);

    // 6. V操作（释放）
    struct sembuf v = {0, 1, 0};
    semop(semid, &v, 1);

    // 7. 清理
    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;
}
