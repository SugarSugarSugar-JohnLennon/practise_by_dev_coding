#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <cstddef>
#include <mutex>
#include <vector>
#include <stdexcept>

/**
 * @brief 简易内存池（固定块大小，线程安全，适合小对象频繁分配/释放）
 *
 * 用法：
 *   MemoryPool<64> pool(1024); // 每块64字节，池总共1024块
 *   void* p = pool.allocate();
 *   pool.deallocate(p);
 */
template <size_t BlockSize>
class MemoryPool {
public:
    // 禁用拷贝
    MemoryPool(const MemoryPool&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;

    /**
     * @brief 构造函数
     * @param blockCount 内存池块数
     */
    explicit MemoryPool(size_t blockCount = 1024);

    /**
     * @brief 析构函数
     */
    ~MemoryPool();

    /**
     * @brief 分配一块内存
     * @return 指向内存块的指针
     */
    void* allocate();

    /**
     * @brief 释放一块内存
     * @param ptr 之前分配的指针
     */
    void deallocate(void* ptr);

    /**
     * @brief 获取池中总块数
     */
    size_t totalBlocks() const;

    /**
     * @brief 获取当前空闲块数
     */
    size_t freeBlocks() const;

private:
    struct FreeBlock {
        FreeBlock* next;
    };

    char* pool_;           // 内存池起始地址
    FreeBlock* freeList_;  // 空闲链表头
    size_t blockCount_;    // 总块数
    size_t freeCount_;     // 空闲块数
    mutable std::mutex mtx_;
};

#include "memory_pool.inl"

#endif // MEMORY_POOL_H 