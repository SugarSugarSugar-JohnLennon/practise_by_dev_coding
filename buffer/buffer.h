#ifndef BUFFER_H
#define BUFFER_H

#include <cstring>   //perror
#include <iostream>
#include <unistd.h>  // write
#include <sys/uio.h> //readv
#include <vector> //readv
#include <atomic>
#include <assert.h>

class Buffer {
private:
    char* beginPtr_();
    const char* beginPtr_() const;
    void makespace_(std::size_t len);

    std::vector<char> buffer_;
    std::atomic<std::size_t> readPos_;
    std::atomic<std::size_t> writePos_;

public:
    Buffer(int initBufferSize = 1024);
    ~Buffer() = default;

    std::size_t writableBytes() const;
    std::size_t readableBytes() const;
    std::size_t prependableBytes() const;

    const char* peek() const;
    void ensureWritable(std::size_t len);
    void hasWritten(std::size_t len);

    void retrieve(std::size_t len);
    void retrieveUntil(const char* end);
    void retrieveAll();
    std::string retrieveAllToStr();

    const char* beginWriteConst() const;
    char* beginWrite();

    void append(const std::string& str);
    void append(const char* str, size_t len);
    void append(const void* data, size_t len);
    void append(const Buffer& buff);

    ssize_t readFd(int fd, int* Errno);
    ssize_t writeFd(int fd, int* Errno);
};
#endif // BUFFER_H