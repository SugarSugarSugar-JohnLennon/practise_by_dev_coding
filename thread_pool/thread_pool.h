#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <atomic>

class ThreadPool {
public:
    // 构造函数：创建指定数量的工作线程
    ThreadPool(size_t threads);
    
    // 析构函数：等待所有任务完成并停止所有线程
    ~ThreadPool();
    
    // 禁用拷贝构造和赋值操作
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    
    // 提交任务到线程池，返回future对象
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;
    
    // 获取当前任务队列大小
    size_t getTaskCount() const;
    
    // 获取线程池大小
    size_t getThreadCount() const;
    
    // 检查线程池是否已停止
    bool isStopped() const;
    
    // 等待所有任务完成
    void waitForAll();

private:
    // 工作线程函数
    void workerThread();
    
    // 成员变量
    std::vector<std::thread> workers_;           // 工作线程容器
    std::queue<std::function<void()>> tasks_;    // 任务队列
    
    // 同步原语
    mutable std::mutex queue_mutex_;             // 队列互斥锁
    std::condition_variable condition_;          // 条件变量
    std::atomic<bool> stop_;                     // 停止标志
    
    // 统计信息
    std::atomic<size_t> task_count_;             // 任务计数器
};

// 模板函数实现
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type> {
    
    using return_type = typename std::result_of<F(Args...)>::type;
    
    // 创建packaged_task来包装函数和参数
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    
    std::future<return_type> res = task->get_future();
    
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        
        // 检查线程池是否已停止
        if (stop_) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }
        
        // 将任务添加到队列
        tasks_.emplace([task](){ (*task)(); });
        task_count_++;
    }
    
    // 通知一个等待的线程
    condition_.notify_one();
    
    return res;
}

#endif // THREAD_POOL_H 