# 线程池
---
## 设计目标
- 高性能：最大化CPU利用率，减少线程切换和锁竞争。
- 安全性：线程安全，防止数据竞争和死锁。
- 通用性：支持任意可调用对象（函数、lambda、成员函数、std::function等）。
- 易用性：接口简洁，易于集成和扩展。
- 可扩展性：支持动态调整线程数量。

## 主要组件
#### 任务表示（Task）
- 使用 std::function<void()> 作为任务的通用表示，支持各种可调用对象。
- 支持任务返回值（可选）：通过 std::packaged_task 或 boost::packaged_task，结合 std::future 或 boost::future 获取结果。
#### 任务队列（Task Queue）
- 线程安全队列，支持多线程并发入队和出队。
- 可用 boost::lockfree::queue 或自定义带锁的队列（如基于 std::mutex + std::condition_variable）。
- 支持任务优先级（可选）：可扩展为优先级队列。

#### 工作线程（Worker Threads）
- 线程池内部维护一组工作线程，循环从任务队列取任务并执行。
- 线程的生命周期由线程池统一管理。

#### 线程池管理（Thread Pool Manager）
- 负责线程的创建、销毁、任务分发等。
- 支持动态调整线程数量（可选）。
- 提供任务提交接口（如 submit、enqueue）。
