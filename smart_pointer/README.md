# C++ 智能指针学习项目

本项目旨在通过从零开始实现 C++ 标准库中的智能指针，深入理解现代 C++ 内存管理和资源管理的核心概念。通过实际编码实现，我们可以更好地理解智能指针的内部机制、设计思想和最佳实践。

## 项目概述

智能指针是 C++11 引入的重要特性，它们通过 RAII（Resource Acquisition Is Initialization）模式自动管理动态分配的内存，有效防止内存泄漏和悬空指针问题。本项目实现了三种主要的智能指针：

1. **`MySharedPtr`** - 共享所有权智能指针
2. **`MyWeakPtr`** - 弱引用智能指针  
3. **`MyUniquePtr`** - 独占所有权智能指针

## API 接口文档

### MyUniquePtr<T> 接口

#### 构造函数
```cpp
explicit MyUniquePtr(T* p = nullptr) noexcept;  // 从原始指针构造
MyUniquePtr(MyUniquePtr&& other) noexcept;      // 移动构造
```

#### 赋值操作
```cpp
MyUniquePtr& operator=(MyUniquePtr&& other) noexcept;  // 移动赋值
// 拷贝构造和拷贝赋值被禁用 (= delete)
```

#### 解引用操作
```cpp
T& operator*() const noexcept;   // 解引用操作符
T* operator->() const noexcept;  // 箭头操作符
```

#### 成员函数
```cpp
T* get() const noexcept;                    // 获取原始指针
T* release() noexcept;                      // 释放所有权，返回原始指针
void reset(T* p = nullptr) noexcept;        // 重置管理的对象
void swap(MyUniquePtr& other) noexcept;     // 交换两个 unique_ptr
```

#### 非成员函数
```cpp
template<typename T>
void swap(MyUniquePtr<T>& a, MyUniquePtr<T>& b) noexcept;  // 交换函数
```

### MySharedPtr<T> 接口

#### 构造函数
```cpp
explicit MySharedPtr(T* p = nullptr);                    // 从原始指针构造
MySharedPtr(const MySharedPtr& other) noexcept;          // 拷贝构造
MySharedPtr(MySharedPtr&& other) noexcept;               // 移动构造
explicit MySharedPtr(const MyWeakPtr<T>& other);         // 从 weak_ptr 构造
```

#### 赋值操作
```cpp
MySharedPtr& operator=(const MySharedPtr& other) noexcept;  // 拷贝赋值
MySharedPtr& operator=(MySharedPtr&& other) noexcept;       // 移动赋值
```

#### 解引用操作
```cpp
T& operator*() const noexcept;   // 解引用操作符
T* operator->() const noexcept;  // 箭头操作符
```

#### 成员函数
```cpp
T* get() const noexcept;                    // 获取原始指针
long use_count() const noexcept;            // 获取强引用计数
void reset(T* p = nullptr);                 // 重置管理的对象
void swap(MySharedPtr& other) noexcept;     // 交换两个 shared_ptr
```

### MyWeakPtr<T> 接口

#### 构造函数
```cpp
MyWeakPtr() noexcept;                                    // 默认构造
MyWeakPtr(const MySharedPtr<T>& sp) noexcept;           // 从 shared_ptr 构造
MyWeakPtr(const MyWeakPtr& other) noexcept;              // 拷贝构造
MyWeakPtr(MyWeakPtr&& other) noexcept;                   // 移动构造
```

#### 赋值操作
```cpp
MyWeakPtr& operator=(const MyWeakPtr& other) noexcept;   // 拷贝赋值
MyWeakPtr& operator=(const MySharedPtr<T>& sp) noexcept; // 从 shared_ptr 赋值
MyWeakPtr& operator=(MyWeakPtr&& other) noexcept;        // 移动赋值
```

#### 成员函数
```cpp
bool expired() const noexcept;              // 检查对象是否已销毁
MySharedPtr<T> lock() const noexcept;       // 尝试获取 shared_ptr
long use_count() const noexcept;            // 获取强引用计数
void reset() noexcept;                      // 重置 weak_ptr
void swap(MyWeakPtr& other) noexcept;       // 交换两个 weak_ptr
```

## 使用示例

### MyUniquePtr 使用示例
```cpp
// 创建 unique_ptr
MyUniquePtr<int> ptr1(new int(42));

// 解引用
int value = *ptr1;  // 42
ptr1->someMethod(); // 调用成员函数

// 所有权转移
MyUniquePtr<int> ptr2 = std::move(ptr1);  // ptr1 变为 nullptr

// 重置
ptr2.reset(new int(100));  // 释放旧对象，管理新对象

// 释放所有权
int* raw_ptr = ptr2.release();  // ptr2 变为 nullptr
delete raw_ptr;  // 手动删除
```

### MySharedPtr 使用示例
```cpp
// 创建 shared_ptr
MySharedPtr<std::string> sp1(new std::string("Hello"));

// 拷贝构造（增加引用计数）
MySharedPtr<std::string> sp2 = sp1;  // 引用计数变为 2

// 解引用
std::cout << *sp1 << std::endl;  // "Hello"
sp1->length();  // 调用成员函数

// 检查引用计数
std::cout << "use_count: " << sp1.use_count() << std::endl;  // 2

// 重置
sp1.reset();  // 引用计数减为 1
```

### MyWeakPtr 使用示例
```cpp
// 创建 shared_ptr
MySharedPtr<std::string> sp(new std::string("World"));

// 从 shared_ptr 创建 weak_ptr
MyWeakPtr<std::string> wp = sp;

// 检查是否过期
if (!wp.expired()) {
    // 尝试获取 shared_ptr
    MySharedPtr<std::string> sp2 = wp.lock();
    if (sp2) {
        std::cout << *sp2 << std::endl;  // "World"
    }
}

// shared_ptr 销毁后
sp.reset();
if (wp.expired()) {
    std::cout << "Object has been destroyed" << std::endl;
}
```

### 循环引用解决方案示例
```cpp
struct Node {
    MySharedPtr<Node> next;
    MyWeakPtr<Node> prev;  // 使用 weak_ptr 避免循环引用
    
    Node() { std::cout << "Node created" << std::endl; }
    ~Node() { std::cout << "Node destroyed" << std::endl; }
};

// 创建循环引用
MySharedPtr<Node> node1(new Node());
MySharedPtr<Node> node2(new Node());

node1->next = node2;  // 强引用
node2->prev = node1;  // 弱引用，不会增加引用计数

// 离开作用域时，对象会被正确销毁
```

## 项目结构

```
smart_pointer/
├── README.md              # 本文件
├── w_s_ptr/               # shared_ptr 和 weak_ptr 实现
│   ├── control_block.h    # 控制块实现（引用计数核心）
│   ├── shared_ptr.h       # MySharedPtr 实现
│   ├── weak_ptr.h         # MyWeakPtr 实现
│   ├── main.cpp           # 测试和演示程序
│   ├── CMakeLists.txt     # CMake 构建配置
│   ├── build.sh           # 构建脚本
│   └── README.md          # 详细学习笔记
└── unique_ptr/            # unique_ptr 实现
    ├── unique_ptr.h       # MyUniquePtr 实现
    ├── main.cpp           # 测试和演示程序
    ├── CMakeLists.txt     # CMake 构建配置
    ├── build.sh           # 构建脚本
    └── README.md          # 详细学习笔记
```

## 子项目详解

### 1. `w_s_ptr` - 共享指针与弱指针

**核心概念：**
- **共享所有权**：多个 `MySharedPtr` 可以同时拥有同一个对象
- **引用计数**：通过控制块管理强引用和弱引用计数
- **循环引用解决**：`MyWeakPtr` 用于打破循环引用，防止内存泄漏

**学习重点：**
- 控制块的设计和实现
- 强引用计数与弱引用计数的管理
- 循环引用问题的识别和解决
- 线程安全的引用计数（使用 `std::atomic`）

**运行方式：**
```bash
cd smart_pointer/w_s_ptr
chmod +x build.sh
./build.sh
```

### 2. `unique_ptr` - 独占指针

**核心概念：**
- **独占所有权**：一个对象只能被一个 `MyUniquePtr` 拥有
- **移动语义**：支持所有权的转移，但禁止拷贝
- **RAII 模式**：资源获取即初始化，自动管理生命周期

**学习重点：**
- 独占所有权的设计理念
- 移动构造函数和移动赋值运算符的实现
- 拷贝构造和拷贝赋值的禁用（`= delete`）
- 所有权转移的语义和实现

**运行方式：**
```bash
cd smart_pointer/unique_ptr
chmod +x build.sh
./build.sh
```

## 学习路径建议

### 初学者路径
1. 先学习 `unique_ptr`，理解独占所有权的概念
2. 再学习 `shared_ptr`，理解共享所有权和引用计数
3. 最后学习 `weak_ptr`，理解循环引用问题和解决方案

### 进阶学习路径
1. 深入理解控制块的设计和内存布局
2. 研究线程安全性和原子操作的使用
3. 分析性能开销和优化策略
4. 学习自定义删除器的实现

## 核心知识点

### 内存管理
- **RAII 模式**：资源获取即初始化，析构时自动释放
- **所有权语义**：明确谁负责释放资源
- **异常安全**：智能指针保证异常情况下的资源正确释放

### 现代 C++ 特性
- **移动语义**：高效的所有权转移
- **模板编程**：类型安全的泛型实现
- **原子操作**：线程安全的引用计数
- **删除函数**：显式禁用不需要的操作

### 设计模式
- **RAII**：资源管理
- **引用计数**：共享资源管理
- **观察者模式**：弱引用的实现

## 实际应用场景

### `MyUniquePtr` 适用场景
- 工厂函数返回的对象
- 资源管理类（如文件句柄、网络连接）
- 多态对象的所有权转移
- 避免异常安全问题的资源管理

### `MySharedPtr` 适用场景
- 多个组件共享同一个对象
- 缓存系统中的对象管理
- 观察者模式中的主体对象
- 需要延迟销毁的对象

### `MyWeakPtr` 适用场景
- 解决循环引用问题
- 缓存系统中的弱引用
- 观察者模式中的观察者引用
- 避免悬空指针的引用

## 性能考虑

### 内存开销
- `MyUniquePtr`：几乎无额外开销（仅一个指针）
- `MySharedPtr`：控制块开销（引用计数 + 删除器 + 对象指针）
- `MyWeakPtr`：与 `MySharedPtr` 共享控制块

### 运行时开销
- `MyUniquePtr`：无运行时开销
- `MySharedPtr`：引用计数的原子操作开销
- `MyWeakPtr`：检查对象是否存在的开销

## 最佳实践

1. **优先使用 `MyUniquePtr`**：当不需要共享所有权时
2. **谨慎使用 `MySharedPtr`**：避免不必要的共享
3. **及时使用 `MyWeakPtr`**：解决循环引用问题
4. **避免裸指针**：尽量使用智能指针管理资源
5. **注意线程安全**：多线程环境下的使用

## 扩展学习

完成本项目后，可以进一步学习：

1. **自定义删除器**：为不同类型的资源实现专门的删除逻辑
2. **智能指针的变体**：如 `std::shared_ptr` 的数组版本
3. **内存池**：结合智能指针实现高效的内存管理
4. **垃圾回收**：理解智能指针与垃圾回收的区别
5. **性能分析**：使用工具分析智能指针的性能影响

## 总结

通过实现这些智能指针，我们不仅学习了现代 C++ 的核心特性，更重要的是理解了资源管理的设计哲学。智能指针不仅仅是语法糖，它们体现了 C++ 对资源管理的深度思考，是编写安全、高效 C++ 代码的重要工具。

希望这个项目能够帮助您深入理解智能指针的工作原理，并在实际项目中正确使用它们！
