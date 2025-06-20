#ifndef MY_SHARED_PTR_H
#define MY_SHARED_PTR_H

#include "control_block.h"
#include <iostream>
#include <utility> // For std::swap

template <typename T>
class MyWeakPtr; // Forward declaration

template <typename T>
class MySharedPtr {
public:
    // 构造函数：从原始指针创建
    explicit MySharedPtr(T* p = nullptr) : ptr(p) {
        if (ptr) {
            cb = new ControlBlock<T>(p);
        } else {
            cb = nullptr;
        }
    }

    // 析构函数
    ~MySharedPtr() {
        release();
    }

    // 拷贝构造函数
    MySharedPtr(const MySharedPtr& other) noexcept : ptr(other.ptr), cb(other.cb) {
        if (cb) {
            cb->shared_count++;
        }
    }

    // 移动构造函数
    MySharedPtr(MySharedPtr&& other) noexcept : ptr(other.ptr), cb(other.cb) {
        other.ptr = nullptr;
        other.cb = nullptr;
    }

    // 从 weak_ptr 构造 (这是 weak_ptr.lock() 的关键)
    explicit MySharedPtr(const MyWeakPtr<T>& other);

    // 拷贝赋值运算符
    MySharedPtr& operator=(const MySharedPtr& other) noexcept {
        if (this != &other) {
            // 先释放当前的资源
            release();
            // 再接管新的资源
            ptr = other.ptr;
            cb = other.cb;
            if (cb) {
                cb->shared_count++;
            }
        }
        return *this;
    }

    // 移动赋值运算符
    MySharedPtr& operator=(MySharedPtr&& other) noexcept {
        if (this != &other) {
            release();
            ptr = other.ptr;
            cb = other.cb;
            other.ptr = nullptr;
            other.cb = nullptr;
        }
        return *this;
    }

    // 重置，释放管理的对象
    void reset(T* p = nullptr) {
        release();
        if (p) {
            ptr = p;
            cb = new ControlBlock<T>(p);
        } else {
            ptr = nullptr;
            cb = nullptr;
        }
    }

    // 解引用
    T& operator*() const noexcept { return *ptr; }
    T* operator->() const noexcept { return ptr; }

    // 获取原始指针
    T* get() const noexcept { return ptr; }

    // 获取强引用计数
    long use_count() const noexcept {
        return cb ? cb->shared_count.load() : 0;
    }

    // 交换
    void swap(MySharedPtr& other) noexcept {
        std::swap(ptr, other.ptr);
        std::swap(cb, other.cb);
    }

private:
    void release() {
        if (cb) {
            if (--(cb->shared_count) == 0) {
                cb->dispose(); // 销毁管理的对象
                if (--(cb->weak_count) == 0) {
                    cb->destroy(); // 销毁控制块
                }
            }
        }
    }

    T* ptr;
    ControlBlockBase* cb;

    friend class MyWeakPtr<T>;
};

#endif // MY_SHARED_PTR_H 