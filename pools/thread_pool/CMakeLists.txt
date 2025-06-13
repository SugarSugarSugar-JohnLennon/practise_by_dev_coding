cmake_minimum_required(VERSION 3.10)
project(ThreadPoolTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含头文件目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 源文件
file(GLOB SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# 测试文件
set(TEST_FILE ${CMAKE_SOURCE_DIR}/test/test_thread_pool.cpp)

# 生成可执行文件
add_executable(test_thread_pool ${SRC_FILES} ${TEST_FILE})

# 链接 pthread
target_link_libraries(test_thread_pool pthread)