/**
 * 实现线程池
 * 
 */

#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_


#include <vector>
#include <thread>
#include <atomic>
#include <memory>
#include "task_queue.h"
#include "task.h"
#include "noncopyable.h"

namespace sugar {
    /**
     * 定义线程池工作类型
     * Fixed  : 0  固定线程数，不回收
     * Cached : 1  空闲超过 x 秒回收线程 
     * version 1.0 目前没实现 Cached 类型
     */
    enum class WorkingType {
        Fixed = 0,
        Cached
    };

    class ThreadPool : private NonCopyable {
    public:
        ThreadPool(WorkingType type, size_t thread_num = std::thread::hardware_concurrency());
        ~ThreadPool();
        
        void submit(std::shared_ptr<TaskInterface> task);
        void shutdown();
        WorkingType type() const;
        size_t thread_count() const;
        bool is_shutdown() const;
        
    private:
        WorkingType type_;
        size_t thread_num_;
        std::vector<std::thread> workers_;
        TaskQueue<std::shared_ptr<TaskInterface>> task_queue_;
        std::atomic<bool> is_shutdown_{false};

        void worker_loop(); // 工作线程主循环
    };
 }

 #endif // _THREAD_POOL_H_