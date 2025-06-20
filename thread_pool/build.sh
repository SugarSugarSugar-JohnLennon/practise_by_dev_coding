#!/bin/bash

echo "=== 构建线程池项目 ==="

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
if [ -f "lib/libthread_pool.a" ]; then
    echo "✓ 线程池库构建成功: lib/libthread_pool.a"
    ls -la lib/
else
    echo "✗ 线程池库构建失败"
    exit 1
fi

if [ -f "bin/simple_example" ]; then
    echo "✓ 简单示例构建成功: bin/simple_example"
else
    echo "✗ 简单示例构建失败"
    exit 1
fi

if [ -f "bin/advanced_test" ]; then
    echo "✓ 高级测试构建成功: bin/advanced_test"
else
    echo "✗ 高级测试构建失败"
    exit 1
fi

echo ""
echo "=== 运行简单示例 ==="
./bin/simple_example

echo ""
echo "=== 运行高级测试 ==="
./bin/advanced_test

echo ""
echo "=== 构建完成 ===" 