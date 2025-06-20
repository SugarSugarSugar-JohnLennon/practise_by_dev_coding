#!/bin/bash
set -e
# 如果build目录不存在，则创建
if [ ! -d "build" ]; then
  mkdir build
fi
cd build
cmake ..
make -j
cd ..
echo "[select] Build finished. Binaries in bin/" 