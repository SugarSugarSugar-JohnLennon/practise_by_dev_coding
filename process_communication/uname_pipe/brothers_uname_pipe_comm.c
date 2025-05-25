#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<limits.h>
#include<string.h>

#define BUFSIZE 4096

int main(void)
{
    int fd[2];
    char buf[BUFSIZE] = "the message to be transferred\n";

    pid_t pid;
    int len;
    if((pipe(fd)) < 0) exit(1);
        

    // create the son process1
    pid = fork();
    if(pid < 0) exit(1);
        
    else if(pid == 0)// son process1
    {
        close(fd[0]);// close read port
        write(fd[1], buf, strlen(buf));// write
        printf("I am son process1(ID:%d), write pipe ok,message is :%s\n", getpid(), buf);
        exit(0);
    }

    // create the son process2
    pid = fork();
    if(pid < 0) exit(1);
    else if(pid == 0)
    {
        char rebef[BUFSIZE] = "";
        close(fd[1]);// close write port
        len = read(fd[0], rebef, BUFSIZE);// read
        printf("I am son process2(ID:%d), read pipe ok,the message is : %s\n", getpid(), rebef);
        write(STDOUT_FILENO, rebef, len);
        exit(0);
    }
    else
    {
        close(fd[0]);// close read port
        close(fd[1]);// close write port
        printf("I am parent process(ID:%d), close my pipe,not send no reveive\n", getpid());
        exit(0);
    }

    return 0;
}