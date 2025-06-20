#include "math_utils.h"
#include <cmath>

namespace MathUtils {
    int add(int a, int b) {
        return a + b;
    }
    
    int subtract(int a, int b) {
        return a - b;
    }
    
    int multiply(int a, int b) {
        return a * b;
    }
    
    double divide(double a, double b) {
        if (b == 0) {
            return 0.0; // 简单处理除零错误
        }
        return a / b;
    }
    
    double power(double base, int exponent) {
        return std::pow(base, exponent);
    }
    
    int factorial(int n) {
        if (n <= 1) return 1;
        int result = 1;
        for (int i = 2; i <= n; ++i) {
            result *= i;
        }
        return result;
    }
    
    bool isPrime(int n) {
        if (n < 2) return false;
        if (n == 2) return true;
        if (n % 2 == 0) return false;
        
        for (int i = 3; i * i <= n; i += 2) {
            if (n % i == 0) return false;
        }
        return true;
    }
} 