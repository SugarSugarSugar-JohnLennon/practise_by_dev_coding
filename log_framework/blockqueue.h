# ifndef BLOCKQUEUE_H
# define BLOCKQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

template<typename T>
class BlockQueue {
public:
    explicit BlockQueue(size_t maxSize = 1000) 
        : maxSize_(maxSize), isClosed_(false) {}
    
    ~BlockQueue() {
        Close();
    }
    
    // 添加元素到队列尾部
    bool push_back(const T& item) {
        std::unique_lock<std::mutex> lock(mtx_);
        
        while (queue_.size() >= maxSize_ && !isClosed_) {
            notFull_.wait(lock);
        }
        
        if (isClosed_) return false;
        
        queue_.push(item);
        notEmpty_.notify_one();
        return true;
    }
    
    // 从队列头部取出元素
    bool pop(T& item) {
        std::unique_lock<std::mutex> lock(mtx_);
        
        while (queue_.empty() && !isClosed_) {
            notEmpty_.wait(lock);
        }
        
        if (isClosed_ && queue_.empty()) return false;
        
        item = queue_.front();
        queue_.pop();
        notFull_.notify_one();
        return true;
    }
    
    // 尝试添加元素（非阻塞）
    bool try_push(const T& item) {
        std::unique_lock<std::mutex> lock(mtx_);
        
        if (queue_.size() >= maxSize_ || isClosed_) return false;
        
        queue_.push(item);
        notEmpty_.notify_one();
        return true;
    }
    
    // 尝试取出元素（非阻塞）
    bool try_pop(T& item) {
        std::unique_lock<std::mutex> lock(mtx_);
        
        if (queue_.empty() || isClosed_) return false;
        
        item = queue_.front();
        queue_.pop();
        notFull_.notify_one();
        return true;
    }
    
    // 获取队列大小
    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return queue_.size();
    }
    
    // 检查队列是否为空
    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return queue_.empty();
    }
    
    // 检查队列是否已满
    bool full() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return queue_.size() >= maxSize_;
    }
    
    // 清空队列
    void clear() {
        std::lock_guard<std::mutex> lock(mtx_);
        while (!queue_.empty()) {
            queue_.pop();
        }
        notFull_.notify_all();
    }
    
    // 刷新队列（等待所有元素被处理）
    void flush() {
        std::unique_lock<std::mutex> lock(mtx_);
        while (!queue_.empty()) {
            notEmpty_.notify_all();
            notFull_.wait(lock);
        }
    }
    
    // 关闭队列
    void Close() {
        std::lock_guard<std::mutex> lock(mtx_);
        isClosed_ = true;
        notEmpty_.notify_all();
        notFull_.notify_all();
    }
    
    // 检查队列是否已关闭
    bool isClosed() const {
        return isClosed_;
    }

private:
    mutable std::mutex mtx_;
    std::condition_variable notEmpty_;
    std::condition_variable notFull_;
    std::queue<T> queue_;
    size_t maxSize_;
    std::atomic<bool> isClosed_;
};

# endif
