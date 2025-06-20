#ifndef MATH_UTILS_H
#define MATH_UTILS_H

namespace MathUtils {
    // 基础数学运算
    int add(int a, int b);
    int subtract(int a, int b);
    int multiply(int a, int b);
    double divide(double a, double b);
    
    // 高级数学运算
    double power(double base, int exponent);
    int factorial(int n);
    bool isPrime(int n);
}

#endif // MATH_UTILS_H 