#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// 定义联合体，用于 semctl
union semun {
    int val;
};

int main() {
    // 1. 创建 key
    key_t key = ftok("semfile", 75);

    // 2. 创建共享内存
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    char* data = (char*) shmat(shmid, NULL, 0);

    // 3. 创建信号量（1个）
    int semid = semget(key, 1, 0666 | IPC_CREAT);

    // 4. 初始化信号量值为 1（可写）
    union semun arg;
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);

    // 5. P操作（等待/占用）
    struct sembuf p = {0, -1, 0};
    semop(semid, &p, 1);

    // 6. 写入数据
    strcpy(data, "Data from writer.");
    printf("Writer: Data written to shared memory.\n");

    // 7. V操作（释放）
    struct sembuf v = {0, 1, 0};
    semop(semid, &v, 1);

    // 8. 清理
    shmdt(data);

    return 0;
}
