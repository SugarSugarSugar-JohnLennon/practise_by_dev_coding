#ifndef CONTROL_BLOCK_H
#define CONTROL_BLOCK_H

#include <atomic>
#include <cstddef>
#include <memory> // For std::default_delete

// 前向声明，解决头文件循环依赖
template <typename T> class MySharedPtr;
template <typename T> class MyWeakPtr;

// 控制块，用于管理引用计数
struct ControlBlockBase {
    std::atomic<long> shared_count;
    std::atomic<long> weak_count;

    ControlBlockBase() : shared_count(1), weak_count(1) {}
    virtual ~ControlBlockBase() = default;

    // 当 shared_ptr 销毁时调用
    virtual void dispose() = 0; 
    // 当最后一个 weak_ptr 销毁时调用，负责删除自身
    virtual void destroy() = 0; 
};

template <typename T, typename Deleter = std::default_delete<T>>
struct ControlBlock : public ControlBlockBase {
    T* ptr;
    Deleter del;

    ControlBlock(T* p, Deleter d = Deleter()) : ptr(p), del(d) {}

    ~ControlBlock() override = default;

    void dispose() override {
        if (ptr) {
            del(ptr);
            ptr = nullptr;
        }
    }

    void destroy() override {
        delete this;
    }
};

#endif // CONTROL_BLOCK_H 