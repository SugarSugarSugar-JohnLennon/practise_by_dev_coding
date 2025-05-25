#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<limits.h>
#include<stdlib.h>
#include<string.h>
#define BUFSIZE PIPE_BUF // PIPE_BUF == 4096

int main(void)
{
    int fd[2];
    char buf[4096] = "message that need to be piped!\n";

    pid_t pid;
    int len;

    // create a pipe
    if((pipe(fd)) < 0)    exit(1);

    // create a son process
    pid = fork();

    if(pid < 0)    exit(1);

    // parent process do this
    else if(pid > 0)
    {
        close(fd[0]);// father process close read port (fd[0])
        write(fd[1], buf, strlen(buf));// write to pipe
        printf("I am parent process(ID:%d),write pipe ok,message is %s\n", getpid(), buf);
        exit(0);
    }
    // son process do this
    else //(pid == 0)
    {
        close(fd[1]);// son process close itself write port (fd[1])
        len = read(fd[0], buf, 4096);// read from pipe
        if(len < 0) exit(1);
        else
        {
            printf("I am son process(ID:%d),read pipe ok,message is: %s\n", getpid(), buf);
            write(STDOUT_FILENO, buf, len);
        }
        exit(0);
    }

}