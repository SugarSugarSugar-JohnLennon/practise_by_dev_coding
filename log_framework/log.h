#ifndef LOG_H
#define LOG_H

#include <string>
#include <memory>
#include <mutex>
#include <thread>
#include <cstdarg>
#include <cstdio>
#include <atomic>
#include "blockqueue.h"

// 日志级别
enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3,
    FATAL = 4
};

// 日志宏定义
#define LOG_DEBUG(fmt, ...) Log::Instance().write(LogLevel::DEBUG, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  Log::Instance().write(LogLevel::INFO, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  Log::Instance().write(LogLevel::WARN, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) Log::Instance().write(LogLevel::ERROR, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) Log::Instance().write(LogLevel::FATAL, fmt, ##__VA_ARGS__)

class Log {
public:
    // 获取单例
    static Log& Instance();

    // 初始化日志系统
    void init(LogLevel level = LogLevel::DEBUG,
              const std::string& logDir = "./log",
              const std::string& logName = "app.log",
              size_t maxQueueSize = 8192,
              size_t maxFileSize = 10 * 1024 * 1024);

    // 写日志（可变参数，支持格式化）
    void write(LogLevel level, const char* fmt, ...);

    // 立即刷新到文件
    void flush();

    // 设置日志级别
    void setLevel(LogLevel level);
    LogLevel getLevel() const;

    // 关闭日志系统
    void close();

    // 是否开启异步
    bool isAsync() const;

private:
    Log();
    ~Log();
    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;

    void asyncWrite_();
    void rollFile_();
    std::string levelToStr_(LogLevel level) const;
    std::string getTimeStr_() const;
    std::string getFileName_() const;

    FILE* fp_;
    std::string logDir_;
    std::string logName_;
    size_t maxFileSize_;
    size_t curFileSize_;
    int fileIndex_;
    LogLevel level_;
    std::unique_ptr<BlockQueue<std::string>> queue_;
    std::unique_ptr<std::thread> writeThread_;
    std::mutex mtx_;
    std::atomic<bool> isOpen_;
    std::atomic<bool> isAsync_;
};

#endif // LOG_H