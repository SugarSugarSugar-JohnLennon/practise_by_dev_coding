#include <iostream>

// 懒汉式的单例模式
//懒汉式的单例模式在多线程环境中可能会存在竞态条件问题。
// 因为多个线程可能同时检测到实例为nullptr，然后都尝试创建一个新的实例。
// 为了确保线程安全，可以使用不同的线程安全机制来修复这个问题。


class singleton
{
public:
    static singleton& getInstance() 
    {
        if(instance == nullptr) instance = new singleton();
        return *instance;
    }

// 私有的属于类的实例instance，唯一可创建的实例，因为类外无法private调用构造。
private:
    static singleton* instance;

// 私有构造+拷贝
private:
    singleton() {
        std::cout << "singleton is created, no more creating!" << std::endl;
    }
    singleton(const singleton&) = delete;
    singleton& operator=(const singleton&) = delete;
};

singleton* singleton::instance = nullptr;