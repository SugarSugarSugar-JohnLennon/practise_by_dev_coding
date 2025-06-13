#include "thread_pool.h"

#include <stdexcept> 
#include <iostream> 

namespace sugar {
    ThreadPool::ThreadPool(WorkingType type, size_t thread_num)
    : type_(type), thread_num_(thread_num), is_shutdown_(false) {
        for (size_t i = 0; i < thread_num_; ++i) {
            workers_.emplace_back([this]() { this->worker_loop(); });
        }
    }

    ThreadPool::~ThreadPool() {
        shutdown();
    }
    
    void ThreadPool::submit(std::shared_ptr<TaskInterface> task) {
        if (is_shutdown_) {
            std::cerr << "[ThreadPool] submit failed: pool is shutdown." << std::endl;
            throw std::runtime_error("ThreadPool is shutdown, cannot submit new tasks.");
        }
        task_queue_.push(std::move(task));
    }

    void ThreadPool::shutdown() {
        if (is_shutdown_.exchange(true)) return;
        
        for (size_t i = 0; i < workers_.size(); ++i) task_queue_.push(nullptr);

        for (auto& t : workers_) {
            if (t.joinable())  t.join();
        }
        workers_.clear();
    }

    WorkingType ThreadPool::type() const {  return type_;  }

    size_t ThreadPool::thread_count() const {  return thread_num_;  }

    bool ThreadPool::is_shutdown() const {  return is_shutdown_;  }

    // 似有方法 worker_loop 
    void ThreadPool::worker_loop() {
        while (true) {
            std::shared_ptr<TaskInterface> task = task_queue_.pop();
            if (is_shutdown_ || !task) break; // 取到空任务或关闭标志，退出
            try {
                task->run();
            } catch (...) {
                std::cerr << "[ThreadPool] worker run task failed... " << std::endl;
            }
        }
    }
}
