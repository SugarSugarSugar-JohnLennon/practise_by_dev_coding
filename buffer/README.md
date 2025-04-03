这个Buffer类实现了一个高效的内存缓冲区，用于处理I/O操作中的数据读写。它使用vector<char>作为底层存储，并通过两个原子指针readPos_和writePos_来跟踪读写位置，支持自动扩容和空间复用。


# 核心机制：
- 读写指针：readPos_和writePos_分别表示当前读和写的位置。可读数据范围是[readPos_, writePos_)。
- 自动扩容：当写入数据时空间不足，makespace_会将数据移动到缓冲区头部或扩容，确保足够的空间。
- 分散读写：使用readv和writev系统调用提高I/O效率。

# 关键函数：

- readFd(int fd, int* Errno)：从文件描述符读取数据到缓冲区，处理空间不足的情况。

- writeFd(int fd, int* Errno)：将缓冲区数据写入文件描述符。

- append系列函数：向缓冲区追加数据，自动处理空间。

- retrieve系列函数：管理已读数据，调整读指针。