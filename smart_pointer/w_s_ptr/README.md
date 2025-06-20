# MySharedPtr 和 MyWeakPtr 实现学习笔记

本项目旨在从零开始实现一个类似 C++ 标准库中的 `std::shared_ptr` 和 `std::weak_ptr` 的智能指针。通过这个过程，我们可以深入理解它们的工作原理、内部机制，特别是**控制块**、**强引用计数**和**弱引用计数**的概念。

## 核心概念

### 1. `MySharedPtr` (共享指针)

`MySharedPtr` 是一种智能指针，它通过引用计数机制实现了对同一块内存的共享所有权。

- **所有权**: 多个 `MySharedPtr` 实例可以指向并拥有同一个对象。
- **生命周期**: 当最后一个指向对象的 `MySharedPtr` 被销毁或重置时，该对象才会被自动删除。
- **引用计数**: 内部维护一个"强引用计数"（shared count），记录有多少个 `MySharedPtr` 正共享着对象。每次拷贝 `MySharedPtr` 时，计数加一；每次 `MySharedPtr` 被销毁时，计数减一。当计数归零，对象被销毁。

### 2. `MyWeakPtr` (弱指针)

`MyWeakPtr` 也是一种智能指针，但它提供的是一种**非拥有性**的弱引用。

- **无所有权**: `MyWeakPtr` 指向一个由 `MySharedPtr`管理的对象，但它**不**增加对象的引用计数。它像一个"观察者"，不会影响对象的生命周期。
- **解决循环引用**: 这是 `MyWeakPtr` 最重要的用途。当两个对象通过 `MySharedPtr` 相互引用时，会形成一个"引用环"，导致它们的引用计数永远不会归零，从而造成内存泄漏。将其中一个引用改为 `MyWeakPtr` 即可打破这个环。
- **检查对象是否存在**: 通过 `expired()` 方法可以检查所观察的对象是否已经被销毁。
- **安全访问**: `MyWeakPtr` 不能直接访问对象。必须通过 `lock()` 方法将其"提升"为一个 `MyShared.ptr`。如果对象仍然存在，`lock()` 返回一个有效的 `MySharedPtr`；否则，返回一个空的 `MySharedPtr`。这保证了访问的内存安全性。

### 3. 控制块 (Control Block)

这是 `shared_ptr` 和 `weak_ptr` 能够协同工作的核心。

- **独立分配**: 当你用原始指针创建一个 `MySharedPtr` 时，它会在堆上额外分配一块内存，这就是"控制块"。
- **内容**: 控制块通常包含：
  1.  **强引用计数 (shared_count)**: 记录 `MySharedPtr` 的数量。
  2.  **弱引用计数 (weak_count)**: 记录 `MyWeakPtr` 的数量以及存活的 `MySharedPtr` 的数量。
  3.  指向被管理对象的指针（或对象本身）。
  4.  自定义的删除器（如果提供了）。
- **生命周期**:
  - 当 `shared_count` 变为 0 时，执行删除器，**销毁被管理的对象**。
  - 当 `weak_count` 变为 0 时（这意味着 `shared_count` 肯定也为 0），**销毁控制块本身**。

## 文件结构

- `control_block.h`: 定义了控制块的结构，包含 `shared_count` 和 `weak_count`。
- `shared_ptr.h`: `MySharedPtr` 的实现。
- `weak_ptr.h`: `MyWeakPtr` 的实现。
- `main.cpp`: 测试用例，包括基础功能测试和循环引用问题的演示与解决。
- `CMakeLists.txt` 和 `build.sh`: 用于编译和运行项目。

## 如何编译和运行

```bash
# 赋予执行权限
chmod +x build.sh

# 执行脚本
./build.sh
```

脚本会自动完成以下工作：
1. 创建 `build` 目录。
2. 在 `build` 目录中运行 `cmake` 和 `make`。
3. 在项目根目录的 `bin` 文件夹下生成可执行文件 `w_s_ptr_demo`。
4. 运行该可执行文件并输出结果。

## 示例分析：循环引用

在 `main.cpp` 中，我们构建了一个经典的循环引用场景：
- `NodeA` 有一个 `MySharedPtr<NodeB>`。
- `NodeB` 有一个 `MySharedPtr<NodeA>`。

当我们将 `a->b_ptr = b;` 和 `b->a_ptr = a;` 连接起来后，`NodeA` 和 `NodeB` 的强引用计数都变成了 2。当 `main` 函数中的 `a` 和 `b` 离开作用域时，它们的引用计数各自减为 1，但永远无法归零。结果是 `NodeA` 和 `NodeB` 的析构函数都不会被调用，造成了内存泄漏。

通过将 `NodeB` 中的指针改为 `MyWeakPtr<GoodNodeA>`，`b->a_ptr = a;` 这一步**不会增加** `GoodNodeA` 的强引用计数。因此，当 `main` 函数作用域结束时，`a` 的引用计数可以顺利降为 0，`GoodNodeA` 被销毁。`GoodNodeA` 的销毁又会释放它对 `GoodNodeB` 的 `MySharedPtr`，使 `GoodNodeB` 的引用计数也降为 0，从而被销毁。问题解决！ 