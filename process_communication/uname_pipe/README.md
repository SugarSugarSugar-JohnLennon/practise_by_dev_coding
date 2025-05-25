# 匿名管道
Linux中使用pipe()函数创建一个匿名管道，其函数原型为：
```cpp
#include <unistd.h>
int pipe(int fd[2]);
```
创建成功返回0，出错返回1。参数fd[2]是一个长度为2的文件描述符数组，fd[1]是写入端的文件描述符，fd[0]是读出端的文件描述符。

可以使用文件I/O函数read()和write()读管道进行读写，使用close()函数关闭管道两端。

创建匿名管道代码：[create_unamed_pipe.cpp](create_unamed_pipe.cpp)

程序中使用pipe函数建立了一个匿名管道fd，之后向管道一端写入数据并从另一端读出数据，将数据输出到标准输出，在程序的最后使用close函数关闭管道的两端。

匿名管道必须是只能在具有亲缘关系的进程间使用

父子进程代码：[father_son_process_uname_pipe_communication.c](father_son_process_uname_pipe_communication.c)


兄弟进程代码：[brothers_uname_pipe_comm.c](brothers_uname_pipe_comm.c)

---
