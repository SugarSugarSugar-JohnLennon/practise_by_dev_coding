#!/bin/bash

echo "Building Log Framework..."

# 创建构建目录
mkdir -p build
cd build

# 配置和编译
cmake ..
make -j$(nproc)

if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo "Executables created in build/bin/"
    
    # 创建日志目录
    mkdir -p logs
    
    echo ""
    echo "Running simple example..."
    ./bin/simple_example
    
    echo ""
    echo "Running performance test..."
    ./bin/performance_test
    
    echo ""
    echo "Build and test completed!"
else
    echo "Build failed!"
    exit 1
fi 