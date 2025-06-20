#include "log.h"
#include <thread>
#include <chrono>

int main() {
    // 初始化日志系统
    Log::Instance().init(LogLevel::DEBUG, "./logs", "app", 1000, 10 * 1024 * 1024);
    
    LOG_INFO("Application started");
    LOG_DEBUG("Debug message: %d + %d = %d", 1, 2, 3);
    LOG_WARN("Warning: This is a test warning");
    LOG_ERROR("Error occurred: %s", "Something went wrong");
    
    // 测试不同日志级别
    Log::Instance().setLevel(LogLevel::INFO);
    LOG_DEBUG("This debug message should not appear");
    LOG_INFO("This info message should appear");
    
    // 测试异步写入
    for (int i = 0; i < 100; ++i) {
        LOG_INFO("Async log message %d", i);
    }
    
    // 等待异步写入完成
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    LOG_INFO("Application finished");
    
    return 0;
} 