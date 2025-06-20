#ifndef MY_WEAK_PTR_H
#define MY_WEAK_PTR_H

#include "shared_ptr.h" 

template <typename T>
class MyWeakPtr {
public:
    // 默认构造函数
    MyWeakPtr() noexcept : ptr(nullptr), cb(nullptr) {}

    // 从 shared_ptr 构造
    MyWeakPtr(const MySharedPtr<T>& sp) noexcept : ptr(sp.ptr), cb(sp.cb) {
        if (cb) {
            cb->weak_count++;
        }
    }

    // 拷贝构造函数
    MyWeakPtr(const MyWeakPtr& other) noexcept : ptr(other.ptr), cb(other.cb) {
        if (cb) {
            cb->weak_count++;
        }
    }

    // 移动构造函数
    MyWeakPtr(MyWeakPtr&& other) noexcept : ptr(other.ptr), cb(other.cb) {
        other.ptr = nullptr;
        other.cb = nullptr;
    }

    // 析构函数
    ~MyWeakPtr() {
        release();
    }

    // 拷贝赋值运算符
    MyWeakPtr& operator=(const MyWeakPtr& other) noexcept {
        if (this != &other) {
            release();
            ptr = other.ptr;
            cb = other.cb;
            if (cb) {
                cb->weak_count++;
            }
        }
        return *this;
    }

    // 从 shared_ptr 赋值
    MyWeakPtr& operator=(const MySharedPtr<T>& sp) noexcept {
        release();
        ptr = sp.ptr;
        cb = sp.cb;
        if (cb) {
            cb->weak_count++;
        }
        return *this;
    }
    
    // 移动赋值运算符
    MyWeakPtr& operator=(MyWeakPtr&& other) noexcept {
        if (this != &other) {
            release();
            ptr = other.ptr;
            cb = other.cb;
            other.ptr = nullptr;
            other.cb = nullptr;
        }
        return *this;
    }

    // 检查被管理的对象是否已被销毁
    bool expired() const noexcept {
        return !cb || cb->shared_count.load() == 0;
    }

    // 尝试获取一个 shared_ptr
    MySharedPtr<T> lock() const noexcept {
        return expired() ? MySharedPtr<T>() : MySharedPtr<T>(*this);
    }

    // 获取强引用计数
    long use_count() const noexcept {
        return cb ? cb->shared_count.load() : 0;
    }

    // 重置
    void reset() noexcept {
        release();
    }
    
    // 交换
    void swap(MyWeakPtr& other) noexcept {
        std::swap(ptr, other.ptr);
        std::swap(cb, other.cb);
    }

private:
    void release() {
        if (cb) {
            if (--(cb->weak_count) == 0) {
                 // 强引用计数此时必定为0，因为 shared_ptr 销毁时会递减 weak_count
                 // 如果 weak_count 也为0，说明没有 weak_ptr 指向控制块了
                cb->destroy(); // 销毁控制块
            }
        }
        ptr = nullptr;
        cb = nullptr;
    }

    T* ptr;
    ControlBlockBase* cb;

    friend class MySharedPtr<T>;
};

// 实现 MySharedPtr 从 MyWeakPtr 的构造函数
template <typename T>
MySharedPtr<T>::MySharedPtr(const MyWeakPtr<T>& other) {
    if (other.expired()) {
        ptr = nullptr;
        cb = nullptr;
    } else {
        ptr = other.ptr;
        cb = other.cb;
        cb->shared_count++;
    }
}


#endif // MY_WEAK_PTR_H 