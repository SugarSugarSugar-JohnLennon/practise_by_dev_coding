# 高性能C++日志框架

一个现代化的C++异步日志框架，具有高性能、线程安全、文件滚动等特性。

## 特性

- **异步写入**: 使用生产者-消费者模式，避免日志写入阻塞主线程
- **线程安全**: 完全线程安全，支持多线程并发写入
- **文件滚动**: 自动文件大小控制和滚动
- **多级别日志**: DEBUG、INFO、WARN、ERROR、FATAL五个级别
- **格式化输出**: 支持printf风格的格式化
- **高性能**: 优化的内存管理和队列机制
- **易用性**: 简单的API和便捷的宏定义

## 快速开始

### 1. 编译

```bash
cd log_framework
./build.sh
```

### 2. 基本使用

```cpp
#include "log.h"

int main() {
    // 初始化日志系统
    Log::Instance().init(LogLevel::DEBUG, "./logs", "app", 1000, 10 * 1024 * 1024);
    
    // 使用日志宏
    LOG_DEBUG("Debug message: %d", 42);
    LOG_INFO("Application started");
    LOG_WARN("Warning message");
    LOG_ERROR("Error occurred: %s", "Something went wrong");
    LOG_FATAL("Fatal error");
    
    return 0;
}
```

## API文档

### 日志级别

```cpp
enum class LogLevel {
    DEBUG = 0,  // 调试信息
    INFO = 1,   // 一般信息
    WARN = 2,   // 警告信息
    ERROR = 3,  // 错误信息
    FATAL = 4   // 致命错误
};
```

### 初始化

```cpp
void Log::init(LogLevel level,           // 日志级别
               const std::string& logDir, // 日志目录
               const std::string& logName, // 日志文件名
               size_t maxQueueSize,       // 队列最大大小
               size_t maxFileSize);       // 文件最大大小(字节)
```

**参数说明:**
- `level`: 最低记录级别，低于此级别的日志将被过滤
- `logDir`: 日志文件存储目录，会自动创建
- `logName`: 日志文件名（不含扩展名）
- `maxQueueSize`: 异步队列最大容量，建议1000-10000
- `maxFileSize`: 单个日志文件最大大小，建议10MB-100MB

### 日志宏

```cpp
LOG_DEBUG(fmt, ...)  // 调试日志
LOG_INFO(fmt, ...)   // 信息日志
LOG_WARN(fmt, ...)   // 警告日志
LOG_ERROR(fmt, ...)  // 错误日志
LOG_FATAL(fmt, ...)  // 致命错误日志
```

**使用示例:**
```cpp
LOG_INFO("User %s logged in from %s", username.c_str(), ip.c_str());
LOG_ERROR("Failed to connect to database: %s", error_msg.c_str());
LOG_DEBUG("Processing request %d", request_id);
```

### 控制方法

```cpp
// 设置日志级别
void Log::setLevel(LogLevel level);

// 获取当前日志级别
LogLevel Log::getLevel() const;

// 强制刷新缓冲区
void Log::flush();

// 关闭日志系统
void Log::close();

// 检查是否为异步模式
bool Log::isAsync() const;
```

## 高级功能

### 文件滚动

当日志文件达到指定大小时，系统会自动创建新文件：

```
logs/
├── app                    # 当前日志文件
├── app.0.2024-01-15      # 历史日志文件
├── app.1.2024-01-15
└── app.2.2024-01-15
```

### 异步写入

日志框架使用异步写入模式：

1. 日志消息首先写入内存队列
2. 后台线程从队列取出消息写入文件
3. 避免日志写入阻塞主线程

### 线程安全

- 完全线程安全，支持多线程并发写入
- 使用互斥锁保护共享资源
- 条件变量实现高效的线程同步

## 性能测试

运行性能测试：

```bash
./build.sh
```

测试结果示例：
```
Starting performance test...
Threads: 8
Messages per thread: 10000
Total messages: 80000
Test completed in 2345 ms
Messages per second: 34115.14
```

## 配置建议

### 生产环境配置

```cpp
// 高性能配置
Log::Instance().init(LogLevel::INFO,     // 生产环境通常不记录DEBUG
                     "./logs",           // 日志目录
                     "production",       // 日志文件名
                     10000,              // 大队列提高吞吐量
                     50 * 1024 * 1024); // 50MB文件大小
```

### 开发环境配置

```cpp
// 开发环境配置
Log::Instance().init(LogLevel::DEBUG,    // 开发环境记录所有级别
                     "./logs",
                     "development",
                     1000,               // 较小队列
                     10 * 1024 * 1024); // 10MB文件大小
```

## 注意事项

1. **初始化**: 使用日志前必须先调用`init()`方法
2. **资源清理**: 程序结束时调用`close()`确保所有日志写入完成
3. **队列大小**: 队列太小可能导致日志丢失，太大可能占用过多内存
4. **文件权限**: 确保程序有权限创建和写入日志目录
5. **磁盘空间**: 定期清理历史日志文件，避免磁盘空间不足

## 故障排除

### 常见问题

1. **日志文件无法创建**
   - 检查目录权限
   - 确保磁盘空间充足

2. **日志丢失**
   - 增加队列大小
   - 检查日志级别设置

3. **性能问题**
   - 调整队列大小
   - 考虑使用SSD存储
   - 减少日志频率

## 示例代码

### 简单示例 (simple_example.cpp)

展示基本使用方法和不同日志级别。

### 性能测试 (performance_test.cpp)

多线程并发测试，验证框架在高负载下的性能。

## 依赖

- C++14 或更高版本
- pthread 库
- CMake 3.10 或更高版本

## 许可证

MIT License
