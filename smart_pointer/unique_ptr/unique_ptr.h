#ifndef MY_UNIQUE_PTR_H
#define MY_UNIQUE_PTR_H

#include <utility> // For std::move and std::swap
#include <iostream>

template <typename T>
class MyUniquePtr {
public:
    // 构造函数
    explicit MyUniquePtr(T* p = nullptr) noexcept : ptr(p) {}

    // 析构函数
    ~MyUniquePtr() noexcept {
        delete ptr;
    }

    // 禁止拷贝构造
    MyUniquePtr(const MyUniquePtr&) = delete;
    // 禁止拷贝赋值
    MyUniquePtr& operator=(const MyUniquePtr&) = delete;

    // 移动构造函数
    MyUniquePtr(MyUniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr; // 转移所有权
    }

    // 移动赋值运算符
    MyUniquePtr& operator=(MyUniquePtr&& other) noexcept {
        if (this != &other) {
            // 先释放当前持有的对象
            delete ptr;
            // 转移所有权
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    // 解引用
    T& operator*() const noexcept { return *ptr; }
    T* operator->() const noexcept { return ptr; }

    // 获取原始指针
    T* get() const noexcept { return ptr; }

    // 放弃所有权并返回原始指针
    T* release() noexcept {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    // 替换管理的对象
    void reset(T* p = nullptr) noexcept {
        T* old_ptr = ptr;
        ptr = p;
        if (old_ptr) {
            delete old_ptr;
        }
    }

    // 交换
    void swap(MyUniquePtr& other) noexcept {
        std::swap(ptr, other.ptr);
    }

private:
    T* ptr;
};

// 为 MyUniquePtr 提供 swap 的非成员函数版本
template <typename T>
void swap(MyUniquePtr<T>& a, MyUniquePtr<T>& b) noexcept {
    a.swap(b);
}

#endif // MY_UNIQUE_PTR_H 