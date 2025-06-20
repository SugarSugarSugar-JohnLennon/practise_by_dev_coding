#!/bin/bash

# 清理之前的构建
rm -rf build
rm -rf bin

# 创建构建目录
mkdir build
cd build

# 运行 CMake 并构建
cmake ..
make

# 返回上级目录
cd ..

# 运行可执行文件
./bin/w_s_ptr_demo 