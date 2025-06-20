#include "unique_ptr.h"
#include <iostream>
#include <cassert>

struct MyClass {
    MyClass() { std::cout << "MyClass constructed." << std::endl; }
    ~MyClass() { std::cout << "MyClass destructed." << std::endl; }
    void greet() { std::cout << "Hello from MyClass!" << std::endl; }
};

// 函数接收 unique_ptr 的所有权
void takes_ownership(MyUniquePtr<MyClass> p) {
    std::cout << "Inside takes_ownership function." << std::endl;
    p->greet();
} // p 离开作用域，MyClass 对象在此处被销毁

int main() {
    std::cout << "--- Starting main ---" << std::endl;

    // 1. 创建 MyUniquePtr
    MyUniquePtr<MyClass> ptr1(new MyClass());
    assert(ptr1.get() != nullptr);
    std::cout << "ptr1 created." << std::endl;

    // 2. 使用 -> 和 *
    ptr1->greet();
    (*ptr1).greet();

    // 3. 转移所有权
    std::cout << "\n--- Transferring ownership from ptr1 to ptr2 ---" << std::endl;
    MyUniquePtr<MyClass> ptr2 = std::move(ptr1);
    assert(ptr1.get() == nullptr); // ptr1 不再拥有对象
    assert(ptr2.get() != nullptr);
    std::cout << "ptr2 now has ownership." << std::endl;
    ptr2->greet();

    // MyUniquePtr<MyClass> ptr_copy = ptr2; // 编译错误！拷贝构造函数是 delete 的
    // ptr1 = ptr2; // 编译错误！拷贝赋值运算符是 delete 的

    // 4. 将所有权转移给函数
    std::cout << "\n--- Transferring ownership to a function ---" << std::endl;
    takes_ownership(std::move(ptr2));
    assert(ptr2.get() == nullptr); // ptr2 也不再拥有对象
    std::cout << "Object should be destructed by now." << std::endl;

    // 5. 使用 reset
    std::cout << "\n--- Testing reset ---" << std::endl;
    MyUniquePtr<MyClass> ptr3(new MyClass());
    ptr3.reset(new MyClass()); // 第一个对象被销毁，ptr3 管理新对象
    ptr3.reset(); // 第二个对象被销毁，ptr3 变成空指针
    assert(ptr3.get() == nullptr);
    std::cout << "ptr3 has been reset." << std::endl;

    // 6. 使用 release
    std::cout << "\n--- Testing release ---" << std::endl;
    MyUniquePtr<MyClass> ptr4(new MyClass());
    MyClass* raw_ptr = ptr4.release(); // ptr4 放弃所有权
    assert(ptr4.get() == nullptr);
    std::cout << "ptr4 released ownership. Manual deletion is now required." << std::endl;
    // 需要手动删除裸指针，否则会内存泄漏
    delete raw_ptr;

    std::cout << "\n--- Exiting main ---" << std::endl;
    return 0;
} 