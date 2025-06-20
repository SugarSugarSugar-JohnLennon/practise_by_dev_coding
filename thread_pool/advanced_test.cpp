#include "thread_pool.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include <algorithm>
#include <future>
#include <cmath>

// 模拟耗时计算任务
double heavyComputation(int iterations) {
    double result = 0.0;
    for (int i = 0; i < iterations; ++i) {
        result += std::sin(i) * std::cos(i);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    return result;
}

// 并行排序任务
std::vector<int> parallelSort(const std::vector<int>& data) {
    std::vector<int> result = data;
    std::sort(result.begin(), result.end());
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    return result;
}

// 矩阵乘法任务
std::vector<std::vector<double>> matrixMultiply(
    const std::vector<std::vector<double>>& a,
    const std::vector<std::vector<double>>& b) {
    
    int rows = a.size();
    int cols = b[0].size();
    int inner = a[0].size();
    
    std::vector<std::vector<double>> result(rows, std::vector<double>(cols, 0.0));
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            for (int k = 0; k < inner; ++k) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    return result;
}

// 生成随机数据
std::vector<int> generateRandomData(size_t size) {
    std::vector<int> data(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);
    
    for (auto& val : data) {
        val = dis(gen);
    }
    return data;
}

int main() {
    std::cout << "=== 线程池高级测试 ===" << std::endl;
    
    // 创建线程池
    ThreadPool pool(std::thread::hardware_concurrency());
    std::cout << "使用 " << pool.getThreadCount() << " 个工作线程" << std::endl;
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // 测试1: 并行计算任务
    std::cout << "\n--- 测试1: 并行计算任务 ---" << std::endl;
    std::vector<std::future<double>> computation_futures;
    
    for (int i = 0; i < 10; ++i) {
        computation_futures.push_back(
            pool.enqueue(heavyComputation, 1000 + i * 100)
        );
    }
    
    // 收集结果
    double total_result = 0.0;
    for (auto& future : computation_futures) {
        total_result += future.get();
    }
    std::cout << "计算任务完成，总结果: " << total_result << std::endl;
    
    // 测试2: 并行排序
    std::cout << "\n--- 测试2: 并行排序 ---" << std::endl;
    std::vector<std::vector<int>> data_sets;
    for (int i = 0; i < 5; ++i) {
        data_sets.push_back(generateRandomData(1000));
    }
    
    std::vector<std::future<std::vector<int>>> sort_futures;
    for (const auto& data : data_sets) {
        sort_futures.push_back(pool.enqueue(parallelSort, data));
    }
    
    // 验证排序结果
    for (size_t i = 0; i < sort_futures.size(); ++i) {
        auto sorted_data = sort_futures[i].get();
        bool is_sorted = std::is_sorted(sorted_data.begin(), sorted_data.end());
        std::cout << "数据集 " << i + 1 << " 排序" 
                  << (is_sorted ? "成功" : "失败") << std::endl;
    }
    
    // 测试3: 任务队列监控
    std::cout << "\n--- 测试3: 任务队列监控 ---" << std::endl;
    
    // 提交大量任务
    std::vector<std::future<int>> task_futures;
    for (int i = 0; i < 50; ++i) {
        task_futures.push_back(
            pool.enqueue([i]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                return i * i;
            })
        );
        
        // 监控队列状态
        if (i % 10 == 0) {
            std::cout << "已提交 " << i << " 个任务，"
                      << "队列中等待: " << pool.getTaskCount() << std::endl;
        }
    }
    
    // 等待所有任务完成
    pool.waitForAll();
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time
    );
    
    std::cout << "\n=== 测试完成 ===" << std::endl;
    std::cout << "总耗时: " << duration.count() << " 毫秒" << std::endl;
    std::cout << "所有任务已成功完成！" << std::endl;
    
    return 0;
} 