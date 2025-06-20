#pragma once
#include <cassert>

// 构造函数
template <size_t BlockSize>
MemoryPool<BlockSize>::MemoryPool(size_t blockCount)
    : pool_(nullptr), freeList_(nullptr), blockCount_(blockCount), freeCount_(blockCount) {
    if (blockCount_ == 0) throw std::invalid_argument("blockCount must be > 0");
    pool_ = new char[BlockSize * blockCount_];
    // 构建空闲链表
    freeList_ = reinterpret_cast<FreeBlock*>(pool_);
    FreeBlock* curr = freeList_;
    for (size_t i = 1; i < blockCount_; ++i) {
        curr->next = reinterpret_cast<FreeBlock*>(pool_ + i * BlockSize);
        curr = curr->next;
    }
    curr->next = nullptr;
}

// 析构函数
template <size_t BlockSize>
MemoryPool<BlockSize>::~MemoryPool() {
    delete[] pool_;
}

// 分配内存
template <size_t BlockSize>
void* MemoryPool<BlockSize>::allocate() {
    std::lock_guard<std::mutex> lock(mtx_);
    if (!freeList_) throw std::bad_alloc();
    FreeBlock* block = freeList_;
    freeList_ = freeList_->next;
    --freeCount_;
    return reinterpret_cast<void*>(block);
}

// 释放内存
template <size_t BlockSize>
void MemoryPool<BlockSize>::deallocate(void* ptr) {
    if (!ptr) return;
    std::lock_guard<std::mutex> lock(mtx_);
    FreeBlock* block = reinterpret_cast<FreeBlock*>(ptr);
    block->next = freeList_;
    freeList_ = block;
    ++freeCount_;
}

template <size_t BlockSize>
size_t MemoryPool<BlockSize>::totalBlocks() const {
    return blockCount_;
}

template <size_t BlockSize>
size_t MemoryPool<BlockSize>::freeBlocks() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return freeCount_;
} 