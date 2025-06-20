cmake_minimum_required(VERSION 3.10)
project(LogFramework)

# 设置C++标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# 查找必要的包
find_package(Threads REQUIRED)

# 包含头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 创建日志库
add_library(log_framework STATIC
    log.cpp
    blockqueue.h
    log.h
)

# 链接线程库
target_link_libraries(log_framework Threads::Threads)

# 创建简单示例
add_executable(simple_example simple_example.cpp)
target_link_libraries(simple_example log_framework Threads::Threads)

# 创建性能测试
add_executable(performance_test performance_test.cpp)
target_link_libraries(performance_test log_framework Threads::Threads)

# 设置输出目录
set_target_properties(simple_example performance_test
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
) 