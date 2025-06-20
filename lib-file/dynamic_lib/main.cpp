#include <iostream>
#include "calculator.h"
#include "geometry.h"

int main() {
    std::cout << "=== 动态库演示程序 ===" << std::endl;
    
    // 测试计算器功能
    std::cout << "\n--- 计算器功能测试 ---" << std::endl;
    std::cout << "10.5 + 3.2 = " << Calculator::add(10.5, 3.2) << std::endl;
    std::cout << "10.5 - 3.2 = " << Calculator::subtract(10.5, 3.2) << std::endl;
    std::cout << "10.5 * 3.2 = " << Calculator::multiply(10.5, 3.2) << std::endl;
    std::cout << "10.5 / 3.2 = " << Calculator::divide(10.5, 3.2) << std::endl;
    std::cout << "5.0 的平方 = " << Calculator::square(5.0) << std::endl;
    std::cout << "3.0 的立方 = " << Calculator::cube(3.0) << std::endl;
    std::cout << "16.0 的平方根 = " << Calculator::sqrt(16.0) << std::endl;
    std::cout << "ln(2.718) = " << Calculator::log(2.718) << std::endl;
    std::cout << "sin(π/2) = " << Calculator::sin(3.14159/2) << std::endl;
    std::cout << "cos(0) = " << Calculator::cos(0) << std::endl;
    
    // 测试几何功能
    std::cout << "\n--- 几何功能测试 ---" << std::endl;
    double radius = 5.0;
    std::cout << "半径为 " << radius << " 的圆:" << std::endl;
    std::cout << "  面积: " << Geometry::circleArea(radius) << std::endl;
    std::cout << "  周长: " << Geometry::circleCircumference(radius) << std::endl;
    
    double width = 4.0, height = 6.0;
    std::cout << "\n宽 " << width << " 高 " << height << " 的矩形:" << std::endl;
    std::cout << "  面积: " << Geometry::rectangleArea(width, height) << std::endl;
    std::cout << "  周长: " << Geometry::rectanglePerimeter(width, height) << std::endl;
    
    double base = 3.0, triHeight = 4.0;
    std::cout << "\n底 " << base << " 高 " << triHeight << " 的三角形:" << std::endl;
    std::cout << "  面积: " << Geometry::triangleArea(base, triHeight) << std::endl;
    std::cout << "  周长: " << Geometry::trianglePerimeter(3.0, 4.0, 5.0) << std::endl;
    
    double sphereRadius = 3.0;
    std::cout << "\n半径为 " << sphereRadius << " 的球体:" << std::endl;
    std::cout << "  体积: " << Geometry::sphereVolume(sphereRadius) << std::endl;
    std::cout << "  表面积: " << Geometry::sphereSurfaceArea(sphereRadius) << std::endl;
    
    double cubeSide = 2.0;
    std::cout << "\n边长为 " << cubeSide << " 的立方体:" << std::endl;
    std::cout << "  体积: " << Geometry::cubeVolume(cubeSide) << std::endl;
    std::cout << "  表面积: " << Geometry::cubeSurfaceArea(cubeSide) << std::endl;
    
    return 0;
} 