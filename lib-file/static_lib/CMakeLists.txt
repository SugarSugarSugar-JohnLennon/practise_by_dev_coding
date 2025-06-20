cmake_minimum_required(VERSION 3.10)
project(StaticLibDemo)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 创建静态库
add_library(static_utils STATIC
    math_utils.cpp
    string_utils.cpp
)

# 设置静态库的属性
set_target_properties(static_utils PROPERTIES
    OUTPUT_NAME "static_utils"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# 创建可执行文件
add_executable(static_demo main.cpp)

# 链接静态库到可执行文件
target_link_libraries(static_demo static_utils)

# 设置可执行文件的输出目录
set_target_properties(static_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# 添加头文件路径
target_include_directories(static_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(static_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# 打印构建信息
message(STATUS "构建静态库: ${CMAKE_BINARY_DIR}/lib/libstatic_utils.a")
message(STATUS "构建可执行文件: ${CMAKE_BINARY_DIR}/bin/static_demo") 