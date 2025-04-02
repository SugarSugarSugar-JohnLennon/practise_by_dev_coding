#include <iostream>
#include "singleton.h"

int main() {
    std::cout << "…………………………开始测试单例模式…………………………" << std::endl;
    singleton& singleton1 = singleton::getInstance();
    singleton& singleton2 = singleton::getInstance();

    // 输出地址，确保两次获取的是同一个实例
    std::cout << "Singleton 1 address: " << &singleton1 << std::endl;
    std::cout << "Singleton 2 address: " << &singleton2 << std::endl;
    
    if(&singleton1 == &singleton2) std::cout << "地址一样" << std::endl;
    else std::cout << "地址不同" << std::endl;
    return 0;
}
