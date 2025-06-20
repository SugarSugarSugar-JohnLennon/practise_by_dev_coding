#include "thread_pool.h"
#include <iostream>
#include <chrono>

ThreadPool::ThreadPool(size_t threads) 
    : stop_(false), task_count_(0) {
    for (size_t i = 0; i < threads; ++i) {
        workers_.emplace_back([this] { workerThread(); });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        stop_ = true;
    }
    condition_.notify_all();
    for (std::thread& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void ThreadPool::workerThread() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            condition_.wait(lock, [this] {
                return stop_ || !tasks_.empty();
            });
            if (stop_ && tasks_.empty()) {
                return;
            }
            task = std::move(tasks_.front());
            tasks_.pop();
        }
        try {
            task();
            task_count_--;
        } catch (...) {
            task_count_--;
        }
    }
}

size_t ThreadPool::getTaskCount() const {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    return tasks_.size();
}

size_t ThreadPool::getThreadCount() const {
    return workers_.size();
}

bool ThreadPool::isStopped() const {
    return stop_.load();
}

void ThreadPool::waitForAll() {
    while (true) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            if (tasks_.empty() && task_count_ == 0) {
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
} 