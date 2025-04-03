#include "buffer.h"

Buffer::Buffer(int initBufferSize) : buffer_(initBufferSize), // vector<char>(sz)
                                     readPos_(0), 
                                     writePos_(0) {}

// 可写的数量：buffer大小 - 写下标
std::size_t Buffer::writableBytes() const {  return buffer_.size() - writePos_;  }


// 可读的数量：写下标 - 读下标
std::size_t Buffer::readableBytes() const {  return writePos_ - readPos_;  }

// 可预留空间：已经读过的就没用了，等于读下标
std::size_t Buffer::prependableBytes() const {  return readPos_;  }

const char* Buffer::peek() const {  return &buffer_[readPos_];  }

void Buffer::ensureWritable(std::size_t len) {
    if(len > writableBytes()) makespace_(len);
    assert(len <= writableBytes());
}

void Buffer::hasWritten(std::size_t len) {   writePos_ += len;  }

void Buffer::retrieve(std::size_t len) {  readPos_ += len;  }

// 读取到end位置
void Buffer::retrieveUntil(const char* end) {
    assert(peek() <= end );
    retrieve(end - peek()); // end指针 - 读指针 长度
}

// 取出所有数据，buffer归零，读写下标归零,在别的函数中会用到
void Buffer::retrieveAll() {
    bzero(&buffer_[0], buffer_.size()); // 覆盖原本数据
    readPos_ = writePos_ = 0; // 奇妙的写法
}

// 取出剩余可读的str
std::string Buffer::retrieveAllToStr() {
    std::string str(peek(), readableBytes());
    retrieveAll();
    return str;
}

// 写指针的位置
const char* Buffer::beginWriteConst() const {  return &buffer_[writePos_];  }

char* Buffer::beginWrite() {  return &buffer_[writePos_];  }

// 添加str到缓冲区
void Buffer::append(const char* str, std::size_t len) {
    assert(str);
    ensureWritable(len);   // 确保可写的长度
    std::copy(str, str + len, beginWrite());    // 将str放到写下标开始的地方
    hasWritten(len);    // 移动写下标
}

void Buffer::append(const std::string& str) {  append(str.c_str(), str.size());  }

void append(const void* data, std::size_t len) {  append(static_cast<const char*>(data), len);  }

// 将buffer中的读下标的地方放到该buffer中的写下标位置
void append(const Buffer& buff) {  append(buff.peek(), buff.readableBytes());  }

// 将fd的内容读到缓冲区，即writable的位置
ssize_t Buffer::readFd(int fd, int* Errno) {
    char buff[65535];   // 栈区
    struct iovec iov[2];
    std::size_t writable = writableBytes(); // 先记录能写多少
    // 分散读， 保证数据全部读完
    iov[0].iov_base = beginWrite();
    iov[0].iov_len = writable;
    iov[1].iov_base = buff;
    iov[1].iov_len = sizeof(buff);

    ssize_t len = readv(fd, iov, 2);
    if(len < 0) {
        *Errno = errno;
    } else if(static_cast<size_t>(len) <= writable) {   // 若len小于writable，说明写区可以容纳len
        writePos_ += len;   // 直接移动写下标
    } else {    
        writePos_ = buffer_.size(); // 写区写满了,下标移到最后
        append(buff, static_cast<std::size_t>(len - writable)); // 剩余的长度
    }
    return len;
}

// 将buffer中可读的区域写入fd中
ssize_t Buffer::writeFd(int fd, int* Errno) {
    ssize_t len = write(fd, peek(), readableBytes());
    if(len < 0) {
        *Errno = errno;
        return len;
    } 
    retrieve(len);
    return len;
}

char* Buffer::beginPtr_() {
    return &buffer_[0];
}

const char* Buffer::beginPtr_() const{
    return &buffer_[0];
}

// 扩展空间
void Buffer::makespace_(std::size_t len) {
    if(writableBytes() + prependableBytes() < len) {
        buffer_.resize(writePos_ + len + 1);
    } else {
        size_t readable = readableBytes();
        std::copy(beginPtr_() + readPos_, beginPtr_() + writePos_, beginPtr_());
        readPos_ = 0;
        writePos_ = readable;
        assert(readable == readableBytes());
    }
}
