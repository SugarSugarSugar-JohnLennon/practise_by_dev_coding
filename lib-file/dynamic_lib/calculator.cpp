#include "calculator.h"
#include <cmath>

#define BUILDING_DLL

namespace Calculator {
    double add(double a, double b) {
        return a + b;
    }
    
    double subtract(double a, double b) {
        return a - b;
    }
    
    double multiply(double a, double b) {
        return a * b;
    }
    
    double divide(double a, double b) {
        if (b == 0.0) {
            return 0.0; // 简单处理除零错误
        }
        return a / b;
    }
    
    double square(double x) {
        return x * x;
    }
    
    double cube(double x) {
        return x * x * x;
    }
    
    double sqrt(double x) {
        if (x < 0) return 0.0; // 简单处理负数
        return std::sqrt(x);
    }
    
    double log(double x) {
        if (x <= 0) return 0.0; // 简单处理非正数
        return std::log(x);
    }
    
    double sin(double x) {
        return std::sin(x);
    }
    
    double cos(double x) {
        return std::cos(x);
    }
} 