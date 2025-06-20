#!/bin/bash

echo "=== 构建内存池项目 ==="

mkdir -p build
cd build

echo "配置CMake..."
cmake ..

echo "编译项目..."
make -j$(nproc)

if [ -f "bin/simple_example" ]; then
    echo "✓ 简单示例构建成功: bin/simple_example"
else
    echo "✗ 简单示例构建失败"
    exit 1
fi

if [ -f "bin/stress_test" ]; then
    echo "✓ 压力测试构建成功: bin/stress_test"
else
    echo "✗ 压力测试构建失败"
    exit 1
fi

echo ""
echo "=== 运行简单示例 ==="
./bin/simple_example

echo ""
echo "=== 运行压力测试 ==="
./bin/stress_test

echo ""
echo "=== 构建完成 ===" 