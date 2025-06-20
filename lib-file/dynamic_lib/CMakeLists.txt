cmake_minimum_required(VERSION 3.10)
project(DynamicLibDemo)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置动态库的可见性
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

# 创建动态库
add_library(dynamic_utils SHARED
    calculator.cpp
    geometry.cpp
)

# 设置动态库的属性
set_target_properties(dynamic_utils PROPERTIES
    OUTPUT_NAME "dynamic_utils"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# 为动态库添加编译定义
target_compile_definitions(dynamic_utils PRIVATE BUILDING_DLL)

# 创建可执行文件
add_executable(dynamic_demo main.cpp)

# 链接动态库到可执行文件
target_link_libraries(dynamic_demo dynamic_utils)

# 设置可执行文件的输出目录
set_target_properties(dynamic_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# 添加头文件路径
target_include_directories(dynamic_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(dynamic_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# 在Linux上设置RPATH
if(UNIX AND NOT APPLE)
    set_target_properties(dynamic_demo PROPERTIES
        INSTALL_RPATH "${CMAKE_BINARY_DIR}/lib"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
endif()

# 打印构建信息
message(STATUS "构建动态库: ${CMAKE_BINARY_DIR}/lib/libdynamic_utils.so")
message(STATUS "构建可执行文件: ${CMAKE_BINARY_DIR}/bin/dynamic_demo") 