#include "log.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>

Log::Log() : fp_(nullptr), maxFileSize_(10 * 1024 * 1024), curFileSize_(0), 
             fileIndex_(0), level_(LogLevel::DEBUG), isOpen_(false), isAsync_(false) {
}

Log::~Log() {
    close();
}

Log& Log::Instance() {
    static Log instance;
    return instance;
}

void Log::init(LogLevel level, const std::string& logDir, const std::string& logName,
               size_t maxQueueSize, size_t maxFileSize) {
    level_ = level;
    logDir_ = logDir;
    logName_ = logName;
    maxFileSize_ = maxFileSize;
    
    // 创建日志目录
    mkdir(logDir_.c_str(), 0755);
    
    // 打开日志文件
    std::string fileName = getFileName_();
    fp_ = fopen(fileName.c_str(), "a");
    if (!fp_) {
        std::cerr << "Failed to open log file: " << fileName << std::endl;
        return;
    }
    
    // 获取当前文件大小
    fseek(fp_, 0, SEEK_END);
    curFileSize_ = ftell(fp_);
    
    // 创建异步队列和线程
    queue_ = std::make_unique<BlockQueue<std::string>>(maxQueueSize);
    isAsync_ = true;
    isOpen_ = true;
    
    // 启动异步写线程
    writeThread_ = std::make_unique<std::thread>(&Log::asyncWrite_, this);
    
    LOG_INFO("Log system initialized successfully");
}

void Log::write(LogLevel level, const char* fmt, ...) {
    if (level < level_ || !isOpen_) return;
    
    std::string timeStr = getTimeStr_();
    std::string levelStr = levelToStr_(level);
    
    // 格式化日志内容
    char buffer[4096];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    
    std::string logMsg = timeStr + " " + buffer;
    
    if (isAsync_) {
        // 异步写入队列
        queue_->push_back(logMsg);
    } else {
        // 同步直接写入文件
        std::lock_guard<std::mutex> lock(mtx_);
        if (fp_) {
            fputs(logMsg.c_str(), fp_);
            fflush(fp_);
            curFileSize_ += logMsg.length();
            
            if (curFileSize_ >= maxFileSize_) {
                rollFile_();
            }
        }
    }
}

void Log::flush() {
    if (isAsync_ && queue_) {
        queue_->flush();
    } else if (fp_) {
        fflush(fp_);
    }
}

void Log::setLevel(LogLevel level) {
    level_ = level;
}

LogLevel Log::getLevel() const {
    return level_;
}

void Log::close() {
    isOpen_ = false;
    
    if (isAsync_ && queue_) {
        queue_->Close();
    }
    
    if (writeThread_ && writeThread_->joinable()) {
        writeThread_->join();
    }
    
    if (fp_) {
        fclose(fp_);
        fp_ = nullptr;
    }
}

bool Log::isAsync() const {
    return isAsync_;
}

void Log::asyncWrite_() {
    std::string logMsg;
    while (isOpen_) {
        if (queue_->pop(logMsg)) {
            std::lock_guard<std::mutex> lock(mtx_);
            if (fp_) {
                fputs(logMsg.c_str(), fp_);
                fflush(fp_);
                curFileSize_ += logMsg.length();
                
                if (curFileSize_ >= maxFileSize_) {
                    rollFile_();
                }
            }
        }
    }
    
    // 处理剩余日志
    while (queue_->pop(logMsg)) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (fp_) {
            fputs(logMsg.c_str(), fp_);
        }
    }
}

void Log::rollFile_() {
    if (!fp_) return;
    
    fclose(fp_);
    
    // 生成新文件名
    std::string oldName = getFileName_();
    std::string newName = logDir_ + "/" + logName_ + "." + 
                         std::to_string(fileIndex_++) + "." + 
                         getTimeStr_().substr(0, 10);
    
    rename(oldName.c_str(), newName.c_str());
    
    // 打开新文件
    std::string fileName = getFileName_();
    fp_ = fopen(fileName.c_str(), "a");
    curFileSize_ = 0;
    
    LOG_INFO("Log file rolled: %s -> %s", oldName.c_str(), newName.c_str());
}

std::string Log::levelToStr_(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO ";
        case LogLevel::WARN:  return "WARN ";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

std::string Log::getTimeStr_() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

std::string Log::getFileName_() const {
    return logDir_ + "/" + logName_;
}
