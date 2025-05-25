#ifndef RECOMMENDED_SINGLETON_H
#define RECOMMENDED_SINGLETON_H

#include <mutex>

class Singleton
{
public:
    // 获取单例实例的接口
    static Singleton& getInstance()
    {
        static Singleton instance;  // 局部静态变量，C++11 起线程安全
        return instance;
    }

    // 示例方法
    void doSomething()
    {
        std::lock_guard<std::mutex> lock(s_mtx);
        // 执行实际逻辑
    }

private:
    // 构造函数私有化
    Singleton() = default;

    // 析构函数私有化
    ~Singleton() = default;

    // 禁用拷贝构造和赋值运算
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // 成员变量（如有需要）
    static std::mutex s_mtx;
};

// 静态成员变量定义
std::mutex Singleton::s_mtx;

#endif // RECOMMENDED_SINGLETON_H
