#!/bin/bash

echo "=== 构建静态库演示 ==="

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
if [ -f "lib/libstatic_utils.a" ]; then
    echo "✓ 静态库构建成功: lib/libstatic_utils.a"
    ls -la lib/
else
    echo "✗ 静态库构建失败"
    exit 1
fi

if [ -f "bin/static_demo" ]; then
    echo "✓ 可执行文件构建成功: bin/static_demo"
    ls -la bin/
else
    echo "✗ 可执行文件构建失败"
    exit 1
fi

echo ""
echo "=== 运行演示程序 ==="
./bin/static_demo

echo ""
echo "=== 构建完成 ===" 