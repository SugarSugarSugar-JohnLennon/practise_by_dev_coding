#!/bin/bash

echo "=== 构建动态库演示 ==="

# 创建构建目录
mkdir -p build
cd build

# 运行CMake配置
echo "配置CMake..."
cmake ..

# 编译
echo "编译项目..."
make -j$(nproc)

# 检查构建结果
if [ -f "lib/libdynamic_utils.so" ]; then
    echo "✓ 动态库构建成功: lib/libdynamic_utils.so"
    ls -la lib/
else
    echo "✗ 动态库构建失败"
    exit 1
fi

if [ -f "bin/dynamic_demo" ]; then
    echo "✓ 可执行文件构建成功: bin/dynamic_demo"
    ls -la bin/
else
    echo "✗ 可执行文件构建失败"
    exit 1
fi

echo ""
echo "=== 运行演示程序 ==="
# 设置库路径并运行
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/lib
./bin/dynamic_demo

echo ""
echo "=== 构建完成 ===" 