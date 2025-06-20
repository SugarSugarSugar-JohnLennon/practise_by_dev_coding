#include "weak_ptr.h" // 包含 weak_ptr.h 就会自动包含 shared_ptr.h
#include <iostream>
#include <cassert>

struct TestStruct {
    int value;
    TestStruct(int v) : value(v) { std::cout << "TestStruct(" << value << ") constructed." << std::endl; }
    ~TestStruct() { std::cout << "TestStruct(" << value << ") destructed." << std::endl; }
};

void test_basic_shared_ptr() {
    std::cout << "\n--- Running test_basic_shared_ptr ---" << std::endl;
    MySharedPtr<TestStruct> sp1(new TestStruct(100));
    assert(sp1.use_count() == 1);
    std::cout << "sp1 use_count: " << sp1.use_count() << std::endl;

    {
        MySharedPtr<TestStruct> sp2 = sp1;
        assert(sp1.use_count() == 2);
        assert(sp2.use_count() == 2);
        std::cout << "sp1 use_count (after sp2 copy): " << sp1.use_count() << std::endl;
        assert(sp1.get() == sp2.get());
    }

    assert(sp1.use_count() == 1);
    std::cout << "sp1 use_count (after sp2 goes out of scope): " << sp1.use_count() << std::endl;

    MySharedPtr<TestStruct> sp3(new TestStruct(200));
    sp1 = sp3;
    assert(sp1.use_count() == 2);
    assert(sp3.use_count() == 2);
    assert(sp1.get()->value == 200);

    sp1.reset();
    assert(sp1.use_count() == 0);
    assert(sp3.use_count() == 1);
    std::cout << "--- Finished test_basic_shared_ptr ---" << std::endl;
}

void test_weak_ptr() {
    std::cout << "\n--- Running test_weak_ptr ---" << std::endl;
    MyWeakPtr<TestStruct> wp;
    {
        MySharedPtr<TestStruct> sp(new TestStruct(300));
        wp = sp;
        assert(sp.use_count() == 1);
        assert(wp.use_count() == 1);
        assert(!wp.expired());

        MySharedPtr<TestStruct> sp_from_wp = wp.lock();
        assert(sp.use_count() == 2);
        assert(sp_from_wp.get() != nullptr);
        assert(sp_from_wp->value == 300);
        std::cout << "Locked weak_ptr successfully. Value: " << sp_from_wp->value << std::endl;
    }

    assert(wp.expired());
    std::cout << "shared_ptr out of scope. weak_ptr is now expired." << std::endl;
    MySharedPtr<TestStruct> sp_fail = wp.lock();
    assert(sp_fail.get() == nullptr);
    std::cout << "Locking expired weak_ptr failed as expected." << std::endl;
    std::cout << "--- Finished test_weak_ptr ---" << std::endl;
}


// --- 循环引用演示 ---

struct NodeB; // 前向声明

struct NodeA {
    MySharedPtr<NodeB> b_ptr;
    NodeA() { std::cout << "NodeA constructor" << std::endl; }
    ~NodeA() { std::cout << "NodeA destructor" << std::endl; }
};

struct NodeB {
    MySharedPtr<NodeA> a_ptr;
    NodeB() { std::cout << "NodeB constructor" << std::endl; }
    ~NodeB() { std::cout << "NodeB destructor" << std::endl; }
};

void test_circular_dependency_issue() {
    std::cout << "\n--- Running test_circular_dependency_issue ---" << std::endl;
    std::cout << "Creating two nodes that point to each other with MySharedPtr..." << std::endl;
    
    MySharedPtr<NodeA> a(new NodeA());
    MySharedPtr<NodeB> b(new NodeB());

    std::cout << "NodeA use_count before linking: " << a.use_count() << std::endl; // 1
    std::cout << "NodeB use_count before linking: " << b.use_count() << std::endl; // 1

    a->b_ptr = b;
    b->a_ptr = a;

    std::cout << "NodeA use_count after linking: " << a.use_count() << std::endl; // 2
    std::cout << "NodeB use_count after linking: " << b.use_count() << std::endl; // 2
    
    std::cout << "Leaving scope. Watch for destructors not being called..." << std::endl;
    // a 和 b 离开作用域，它们的引用计数从 2 降到 1，但不会降到 0
    // 因此 NodeA 和 NodeB 的析构函数不会被调用，导致内存泄漏
}


// --- 使用 weak_ptr 解决循环引用 ---

struct GoodNodeB; // 前向声明

struct GoodNodeA {
    MySharedPtr<GoodNodeB> b_ptr;
    GoodNodeA() { std::cout << "GoodNodeA constructor" << std::endl; }
    ~GoodNodeA() { std::cout << "GoodNodeA destructor" << std::endl; }
};

struct GoodNodeB {
    MyWeakPtr<GoodNodeA> a_ptr; // 使用 MyWeakPtr 打破循环
    GoodNodeB() { std::cout << "GoodNodeB constructor" << std::endl; }
    ~GoodNodeB() { std::cout << "GoodNodeB destructor" << std::endl; }
    void check_a() {
        if (!a_ptr.expired()) {
            std::cout << "GoodNodeB can still access GoodNodeA." << std::endl;
        } else {
            std::cout << "GoodNodeB can no longer access GoodNodeA." << std::endl;
        }
    }
};

void test_circular_dependency_solution() {
    std::cout << "\n--- Running test_circular_dependency_solution ---" << std::endl;
    std::cout << "Creating two nodes with one weak link..." << std::endl;
    
    MySharedPtr<GoodNodeA> a(new GoodNodeA());
    MySharedPtr<GoodNodeB> b(new GoodNodeB());

    std::cout << "GoodNodeA use_count before linking: " << a.use_count() << std::endl; // 1
    std::cout << "GoodNodeB use_count before linking: " << b.use_count() << std::endl; // 1

    a->b_ptr = b; // A 指向 B (强引用)
    b->a_ptr = a; // B 指向 A (弱引用)

    std::cout << "GoodNodeA use_count after linking: " << a.use_count() << std::endl; // 1 (因为 weak_ptr 不增加引用计数)
    std::cout << "GoodNodeB use_count after linking: " << b.use_count() << std::endl; // 2

    b->check_a();
    
    std::cout << "Leaving scope. Destructors should be called correctly." << std::endl;
}


int main() {
    test_basic_shared_ptr();
    test_weak_ptr();

    test_circular_dependency_issue();
    std::cout << "NOTE: If you don't see NodeA/NodeB destructors, the leak is confirmed." << std::endl;

    test_circular_dependency_solution();
    std::cout << "NOTE: You should see GoodNodeA/GoodNodeB destructors now." << std::endl;

    return 0;
} 