# 单例模式
单例模式是确保一个类只有一个实例，并提供一个全局访问点，使得程序的各个部分可以共享这个实例。单例模式的主要目的是限制一个类的实例化过程，确保在运行时只能存在一个实例，并提供一种全局访问方式。

```cpp
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
```

考虑到可能多线程时，判断instance为nullptr的竞争情况，于是上锁
```cpp
class Singleton {
public:
    static Singleton& getInstance() {
        if (instance == nullptr) {
            std::lock_guard<std::mutex> lock(mutex);  
            if (instance == nullptr) {
                instance = new Singleton();
            }
        }
        return *instance;
    }

private:
    Singleton() {
        std::cout << "Singleton instance created." << std::endl;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static Singleton* instance;
    static std::mutex mutex;  
};
Singleton* Singleton::instance = nullptr;
std::mutex Singleton::mutex;
```

## 为什么要使用单例模式？
- 全局访问点： 单例模式提供了一个全局的访问点，使得程序的各个部分可以轻松地访问到相同的实例。这有助于统一管理资源或状态。
- 资源共享： 当一个类的实例需要被多个部分共享时，使用单例模式可以避免重复创建实例，节省系统资源。
- 配置管理： 单例模式常用于管理系统的配置信息，确保在整个程序运行期间只有一个配置实例，并可以被所有模块访问。
- 线程池、连接池等管理对象的情况： 在需要限制系统中某些资源的数量时，可以使用单例模式来管理这些资源的实例。
- 日志记录： 单例模式可以用于记录系统中的日志信息，确保只有一个日志实例，并提供全局的访问点，方便记录系统的运行状态。
- 避免重复创建开销大的对象： 当对象的创建和销毁具有较大的开销时，使用单例模式可以减少创建次数，提高性能。
- 虽然单例模式有其用途，但需要慎重使用，因为它可能导致全局状态的存在，使得代码的复杂性增加。在一些情况下，依赖注入、工厂模式等也是可以考虑的替代方案。

## 单例模式的缺点
尽管单例模式在某些情况下是有用的，但它也有一些缺点，需要在使用时考虑：
- 全局状态： 单例模式引入了全局状态，这可能导致代码的复杂性增加。因为任何部分都可以访问单例实例，可能导致难以追踪和理解的依赖关系。
- 隐藏依赖关系： 使用单例模式可能隐藏了类之间的依赖关系，因为单例实例可以在任何地方被访问。这使得代码的耦合度增加，降低了灵活性。
- 测试困难： 单例模式可能会使代码难以测试。因为单例实例是全局可访问的，难以在测试中用模拟对象替代实际的单例实例。
- 违反单一职责原则： 单例对象通常负责自己的创建、管理和业务逻辑，可能导致违反单一职责原则。这使得代码难以维护和扩展。
- 可能引发并发问题： 在多线程环境中，懒汉式的单例模式可能引发竞态条件问题，需要特殊处理以确保线程安全。
- 阻碍扩展： 单例模式的实现方式可能会阻碍程序的扩展。例如，如果需要从单例模式切换到多例模式或其他设计模式，可能需要修改大量代码。


网上看到的写法 存一下 以后看：

```cpp
class Singleton {
public:
    // 获取单例实例的静态方法
    static Singleton& getInstance() {
        std::call_once(initFlag, &Singleton::initInstance);
        return *instance;
    }
private:
    Singleton() {
        std::cout << "Singleton instance created." << std::endl;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // 初始化单例实例的函数
    static void initInstance() {
        instance = new Singleton();
    }

    // 静态成员变量，用于保存唯一实例
    static Singleton* instance;
    static std::once_flag initFlag;  // 标记是否已经初始化过
};

Singleton* Singleton::instance = nullptr;
std::once_flag Singleton::initFlag;
```                   
[原文链接：https://blog.csdn.net/a1379292747/article/details/135264703](https://blog.csdn.net/a1379292747/article/details/135264703)
