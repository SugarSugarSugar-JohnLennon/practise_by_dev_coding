#ifndef GEOMETRY_H
#define GEOMETRY_H

#ifdef _WIN32
    #ifdef BUILDING_DLL
        #define EXPORT __declspec(dllexport)
    #else
        #define EXPORT __declspec(dllimport)
    #endif
#else
    #define EXPORT __attribute__((visibility("default")))
#endif

namespace Geometry {
    // 圆形相关计算
    EXPORT double circleArea(double radius);
    EXPORT double circleCircumference(double radius);
    
    // 矩形相关计算
    EXPORT double rectangleArea(double width, double height);
    EXPORT double rectanglePerimeter(double width, double height);
    
    // 三角形相关计算
    EXPORT double triangleArea(double base, double height);
    EXPORT double trianglePerimeter(double a, double b, double c);
    
    // 球体相关计算
    EXPORT double sphereVolume(double radius);
    EXPORT double sphereSurfaceArea(double radius);
    
    // 立方体相关计算
    EXPORT double cubeVolume(double side);
    EXPORT double cubeSurfaceArea(double side);
}

#endif // GEOMETRY_H 