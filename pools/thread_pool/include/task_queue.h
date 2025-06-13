/**
 * 任务队列
 * 
 * 目标：
 * 1. 线程安全：支持多线程安全地入队（push）和出队（pop）。
 * 2. 阻塞与非阻塞：支持阻塞等待（队列为空时等待新任务），也可支持非阻塞尝试获取任务。
 * 3. 泛型：支持存放任意类型（如 std::shared_ptr<TaskInterface>）。
 * 4. 高效：尽量减少锁竞争。
 */

#ifndef _TASK_QUEUE_H_
#define _TASK_QUEUE_H_

#include <queue>
#include <mutex>
#include <condition_variable>

namespace sugar {
    // ====== 下面是任务队列的声明 ======
    template <typename T>
    class TaskQueue {
    public:
        TaskQueue() = default;
        ~TaskQueue() = default;

        void push(const T& item);
        void push(T&& item);
        T pop();
        bool try_pop(T& item);
        size_t size() const;
        bool empty() const;

    private:
        std::queue<T> task_queue_;
        mutable std::mutex mutex_;
        std::condition_variable cond_;
    };

    // ====== 下面是成员函数实现 ======
    template <typename T>
    size_t TaskQueue<T>::size() const {  
        std::lock_guard<std::mutex> lock(mutex_); 
        return task_queue_.size();
    }

    template <typename T>
    bool TaskQueue<T>::empty() const { 
        std::lock_guard<std::mutex> lock(mutex_); 
        return task_queue_.empty();  
    }

    template <typename T>
    void TaskQueue<T>::push(const T& item) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            task_queue_.push(item);
        }
        cond_.notify_one(); // 唤醒一个等待线程
    }

    template <typename T>
    void TaskQueue<T>::push(T&& item) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            task_queue_.push(std::move(item));
        }
        cond_.notify_one(); 
    }

    template <typename T>
    T TaskQueue<T>::pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this](){  return !task_queue_.empty();  }); // 等待入队唤醒。
        T item = std::move(task_queue_.front());
        task_queue_.pop();
        return item;
    }

    /* 尝试取出一个元素，如果队列为空则立即返回 false，否则取出队首元素并返回 true。 */
    template <typename T>
    bool TaskQueue<T>::try_pop(T& item) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (task_queue_.empty()) return false;
        item = std::move(task_queue_.front());
        task_queue_.pop();

        return true;
    }
}

 #endif // _TASK_QUEUE_H_