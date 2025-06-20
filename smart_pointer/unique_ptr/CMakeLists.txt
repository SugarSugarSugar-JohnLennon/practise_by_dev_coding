cmake_minimum_required(VERSION 3.10)

project(unique_ptr_demo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(u_ptr_demo main.cpp)

target_include_directories(u_ptr_demo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin) 