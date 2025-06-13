/**
 * 对任务进行封装
 * 1. 任务抽象为Task
 * 2. 重写 run() 对任务进行执行
 * 
 */

#ifndef _TASK_H_
#define _TASK_H_

#include <boost/function.hpp> // boost::function
#include <memory>

namespace sugar {

    // 任务接口 TaskInterface， 实现 run 来 执行具体的任务
    class TaskInterface {
    public:
        TaskInterface() = default;
        virtual ~TaskInterface() = default;
        virtual void run() = 0; 
    };

    template <typename R>
    class Task : public TaskInterface {
    public:
        using func_type = boost::function<R()>; 
        explicit Task(func_type&& func) : func_(std::move(func)) {}
        void run() override {  func_();  }
    private:
        func_type func_; // 任务类型
    };
}

#endif // _TASK_H_