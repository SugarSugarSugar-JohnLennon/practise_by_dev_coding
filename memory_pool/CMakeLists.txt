cmake_minimum_required(VERSION 3.10)
project(MemoryPoolDemo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread")

add_executable(simple_example simple_example.cpp)
add_executable(stress_test stress_test.cpp)

# 添加头文件路径
target_include_directories(simple_example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(stress_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(simple_example pthread)
target_link_libraries(stress_test pthread)

set_target_properties(simple_example PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set_target_properties(stress_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

message(STATUS "构建简单示例: ${CMAKE_BINARY_DIR}/bin/simple_example")
message(STATUS "构建压力测试: ${CMAKE_BINARY_DIR}/bin/stress_test") 