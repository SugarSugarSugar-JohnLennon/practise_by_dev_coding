cmake_minimum_required(VERSION 3.10)
project(ThreadPoolDemo)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread")

# 创建线程池库
add_library(thread_pool_lib STATIC
    thread_pool.cpp
)

# 设置库的属性
set_target_properties(thread_pool_lib PROPERTIES
    OUTPUT_NAME "thread_pool"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# 添加头文件路径
target_include_directories(thread_pool_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 创建简单示例可执行文件
add_executable(simple_example simple_example.cpp)

# 链接线程池库
target_link_libraries(simple_example thread_pool_lib pthread)

# 创建高级测试可执行文件
add_executable(advanced_test advanced_test.cpp)

# 链接线程池库
target_link_libraries(advanced_test thread_pool_lib pthread)

# 设置可执行文件的输出目录
set_target_properties(simple_example PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
set_target_properties(advanced_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# 打印构建信息
message(STATUS "构建线程池库: ${CMAKE_BINARY_DIR}/lib/libthread_pool.a")
message(STATUS "构建简单示例: ${CMAKE_BINARY_DIR}/bin/simple_example")
message(STATUS "构建高级测试: ${CMAKE_BINARY_DIR}/bin/advanced_test") 