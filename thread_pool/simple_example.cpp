#include "thread_pool.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

// 简单的任务函数
int add(int a, int b) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 模拟工作
    return a + b;
}

// 打印信息的任务
void printMessage(const std::string& message) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "线程 " << std::this_thread::get_id() 
              << " 执行任务: " << message << std::endl;
}

// 计算阶乘的任务
int factorial(int n) {
    if (n <= 1) return 1;
    int result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return result;
}

int main() {
    std::cout << "=== 线程池简单示例 ===" << std::endl;
    
    // 创建包含4个工作线程的线程池
    ThreadPool pool(4);
    
    std::cout << "线程池大小: " << pool.getThreadCount() << std::endl;
    
    // 提交一些简单的任务
    auto future1 = pool.enqueue(add, 10, 20);
    auto future2 = pool.enqueue(add, 30, 40);
    auto future3 = pool.enqueue(factorial, 5);
    
    // 提交一些打印任务
    pool.enqueue(printMessage, "任务1");
    pool.enqueue(printMessage, "任务2");
    pool.enqueue(printMessage, "任务3");
    
    // 获取结果
    std::cout << "10 + 20 = " << future1.get() << std::endl;
    std::cout << "30 + 40 = " << future2.get() << std::endl;
    std::cout << "5! = " << future3.get() << std::endl;
    
    // 等待所有任务完成
    pool.waitForAll();
    
    std::cout << "所有任务已完成！" << std::endl;
    
    return 0;
} 