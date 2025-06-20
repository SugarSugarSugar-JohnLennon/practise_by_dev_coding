#include "memory_pool.h"
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include <cstring>

struct TestObj {
    int x;
    char buf[32];
    TestObj(int v, const char* s) : x(v) { strncpy(buf, s, sizeof(buf) - 1); buf[sizeof(buf) - 1] = '\0'; }
};

constexpr size_t blockSize = sizeof(TestObj);
constexpr size_t blockCount = 10000;
constexpr int threadNum = 8;
constexpr int opsPerThread = 100000;

void worker(MemoryPool<blockSize>& pool, std::atomic<int>& allocCount) {
    std::vector<void*> localPtrs;
    for (int i = 0; i < opsPerThread; ++i) {
        void* p = pool.allocate();
        TestObj* obj = new(p) TestObj(i, "stress");
        localPtrs.push_back(p);
        ++allocCount;
        if (localPtrs.size() > 10) {
            void* q = localPtrs.back();
            localPtrs.pop_back();
            TestObj* o = reinterpret_cast<TestObj*>(q);
            o->~TestObj();
            pool.deallocate(q);
        }
    }
    // 释放剩余
    for (void* p : localPtrs) {
        TestObj* o = reinterpret_cast<TestObj*>(p);
        o->~TestObj();
        pool.deallocate(p);
    }
}

int main() {
    std::cout << "=== 内存池压力测试（多线程） ===\n";
    MemoryPool<blockSize> pool(blockCount);
    std::atomic<int> allocCount{0};
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threads;
    for (int i = 0; i < threadNum; ++i) {
        threads.emplace_back(worker, std::ref(pool), std::ref(allocCount));
    }
    for (auto& t : threads) t.join();
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "总分配次数: " << allocCount << std::endl;
    std::cout << "空闲块: " << pool.freeBlocks() << "/" << pool.totalBlocks() << std::endl;
    std::cout << "耗时: " << ms << " ms" << std::endl;
    return 0;
} 