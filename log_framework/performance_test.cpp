#include "log.h"
#include <thread>
#include <chrono>
#include <vector>
#include <iostream>

void worker_thread(int thread_id, int message_count) {
    for (int i = 0; i < message_count; ++i) {
        LOG_INFO("Thread %d: Message %d", thread_id, i);
        
        if (i % 1000 == 0) {
            LOG_DEBUG("Thread %d: Debug checkpoint at %d", thread_id, i);
        }
        
        if (i % 5000 == 0) {
            LOG_WARN("Thread %d: Warning at %d", thread_id, i);
        }
    }
}

int main() {
    const int thread_count = 8;
    const int messages_per_thread = 10000;
    
    std::cout << "Starting performance test..." << std::endl;
    std::cout << "Threads: " << thread_count << std::endl;
    std::cout << "Messages per thread: " << messages_per_thread << std::endl;
    std::cout << "Total messages: " << thread_count * messages_per_thread << std::endl;
    
    // 初始化日志系统
    Log::Instance().init(LogLevel::INFO, "./logs", "perf_test", 10000, 50 * 1024 * 1024);
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // 创建工作线程
    std::vector<std::thread> threads;
    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back(worker_thread, i, messages_per_thread);
    }
    
    // 等待所有线程完成
    for (auto& thread : threads) {
        thread.join();
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    LOG_INFO("Performance test completed");
    LOG_INFO("Total time: %ld ms", duration.count());
    LOG_INFO("Messages per second: %.2f", 
             (thread_count * messages_per_thread * 1000.0) / duration.count());
    
    std::cout << "Test completed in " << duration.count() << " ms" << std::endl;
    std::cout << "Messages per second: " 
              << (thread_count * messages_per_thread * 1000.0) / duration.count() 
              << std::endl;
    
    return 0;
} 