#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main()
{
    int fd[2]; // sender receiver
    char str[256]; // msg

    if((pipe(fd)) < 0) {
        printf("create the pipe failed!\n");
        exit(1);
    }

    write(fd[1], "create the pipe sucessfully!\n", 31);
    read(fd[0], str, sizeof(str));
    printf("%s", str);
    printf("pipe fd  arefd[0]:%d,fd[1]:%d\n", fd[0], fd[1]);
    close(fd[0]);
    close(fd[1]);

    return 0;
}