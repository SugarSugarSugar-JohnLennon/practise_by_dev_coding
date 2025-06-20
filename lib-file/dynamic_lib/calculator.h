#ifndef CALCULATOR_H
#define CALCULATOR_H

#ifdef _WIN32
    #ifdef BUILDING_DLL
        #define EXPORT __declspec(dllexport)
    #else
        #define EXPORT __declspec(dllimport)
    #endif
#else
    #define EXPORT __attribute__((visibility("default")))
#endif

namespace Calculator {
    // 基础计算器功能
    EXPORT double add(double a, double b);
    EXPORT double subtract(double a, double b);
    EXPORT double multiply(double a, double b);
    EXPORT double divide(double a, double b);
    
    // 高级计算器功能
    EXPORT double square(double x);
    EXPORT double cube(double x);
    EXPORT double sqrt(double x);
    EXPORT double log(double x);
    EXPORT double sin(double x);
    EXPORT double cos(double x);
}

#endif // CALCULATOR_H 