#include <iostream>
#include "math_utils.h"
#include "string_utils.h"

int main() {
    std::cout << "=== 静态库演示程序 ===" << std::endl;
    
    // 测试数学工具
    std::cout << "\n--- 数学工具测试 ---" << std::endl;
    std::cout << "10 + 5 = " << MathUtils::add(10, 5) << std::endl;
    std::cout << "10 - 5 = " << MathUtils::subtract(10, 5) << std::endl;
    std::cout << "10 * 5 = " << MathUtils::multiply(10, 5) << std::endl;
    std::cout << "10 / 5 = " << MathUtils::divide(10, 5) << std::endl;
    std::cout << "2^8 = " << MathUtils::power(2, 8) << std::endl;
    std::cout << "5! = " << MathUtils::factorial(5) << std::endl;
    std::cout << "17 是质数吗？ " << (MathUtils::isPrime(17) ? "是" : "否") << std::endl;
    
    // 测试字符串工具
    std::cout << "\n--- 字符串工具测试 ---" << std::endl;
    std::string testStr = "Hello World";
    std::cout << "原字符串: " << testStr << std::endl;
    std::cout << "转大写: " << StringUtils::toUpperCase(testStr) << std::endl;
    std::cout << "转小写: " << StringUtils::toLowerCase(testStr) << std::endl;
    std::cout << "反转: " << StringUtils::reverse(testStr) << std::endl;
    
    std::string csvStr = "apple,banana,orange,grape";
    std::cout << "\n分割字符串 '" << csvStr << "':" << std::endl;
    auto tokens = StringUtils::split(csvStr, ',');
    for (const auto& token : tokens) {
        std::cout << "  - " << token << std::endl;
    }
    
    std::cout << "重新连接: " << StringUtils::join(tokens, " | ") << std::endl;
    
    std::cout << "\n字符串检查:" << std::endl;
    std::cout << "'Hello World' 以 'Hello' 开头？ " 
              << (StringUtils::startsWith(testStr, "Hello") ? "是" : "否") << std::endl;
    std::cout << "'Hello World' 以 'World' 结尾？ " 
              << (StringUtils::endsWith(testStr, "World") ? "是" : "否") << std::endl;
    std::cout << "'Hello World' 包含 'lo'？ " 
              << (StringUtils::contains(testStr, "lo") ? "是" : "否") << std::endl;
    
    return 0;
} 