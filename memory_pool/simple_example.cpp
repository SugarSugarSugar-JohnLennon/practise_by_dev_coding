#include "memory_pool.h"
#include <iostream>
#include <vector>
#include <cstring>

struct MyStruct {
    int a;
    double b;
    char msg[16];
    
    MyStruct(int x, double y, const char* s) : a(x), b(y) {
        strncpy(msg, s, sizeof(msg) - 1);
        msg[sizeof(msg) - 1] = '\0';
    }
};

int main() {
    std::cout << "=== 内存池简单示例 ===\n";
    constexpr size_t blockSize = sizeof(MyStruct);
    MemoryPool<blockSize> pool(10); // 10块

    std::vector<void*> ptrs;
    // 分配5个对象
    for (int i = 0; i < 5; ++i) {
        void* p = pool.allocate();
        MyStruct* obj = new(p) MyStruct(i, i * 1.5, "hello");
        std::cout << "分配: " << obj << " a=" << obj->a << " b=" << obj->b << " msg=" << obj->msg << std::endl;
        ptrs.push_back(p);
    }
    std::cout << "空闲块: " << pool.freeBlocks() << "/" << pool.totalBlocks() << std::endl;

    // 释放所有对象
    for (void* p : ptrs) {
        MyStruct* obj = reinterpret_cast<MyStruct*>(p);
        std::cout << "释放: " << obj << std::endl;
        obj->~MyStruct();
        pool.deallocate(p);
    }
    std::cout << "空闲块: " << pool.freeBlocks() << "/" << pool.totalBlocks() << std::endl;
    return 0;
} 