#include "geometry.h"
#include <cmath>

#define BUILDING_DLL

namespace Geometry {
    const double PI = 3.14159265358979323846;
    
    double circleArea(double radius) {
        if (radius < 0) return 0.0;
        return PI * radius * radius;
    }
    
    double circleCircumference(double radius) {
        if (radius < 0) return 0.0;
        return 2 * PI * radius;
    }
    
    double rectangleArea(double width, double height) {
        if (width < 0 || height < 0) return 0.0;
        return width * height;
    }
    
    double rectanglePerimeter(double width, double height) {
        if (width < 0 || height < 0) return 0.0;
        return 2 * (width + height);
    }
    
    double triangleArea(double base, double height) {
        if (base < 0 || height < 0) return 0.0;
        return 0.5 * base * height;
    }
    
    double trianglePerimeter(double a, double b, double c) {
        if (a < 0 || b < 0 || c < 0) return 0.0;
        // 简单的三角形验证
        if (a + b <= c || a + c <= b || b + c <= a) return 0.0;
        return a + b + c;
    }
    
    double sphereVolume(double radius) {
        if (radius < 0) return 0.0;
        return (4.0 / 3.0) * PI * radius * radius * radius;
    }
    
    double sphereSurfaceArea(double radius) {
        if (radius < 0) return 0.0;
        return 4 * PI * radius * radius;
    }
    
    double cubeVolume(double side) {
        if (side < 0) return 0.0;
        return side * side * side;
    }
    
    double cubeSurfaceArea(double side) {
        if (side < 0) return 0.0;
        return 6 * side * side;
    }
} 