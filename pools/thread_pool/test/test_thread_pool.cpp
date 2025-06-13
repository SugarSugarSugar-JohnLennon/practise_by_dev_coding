#include "../include/thread_pool.h"
#include <iostream>
#include <memory>

class PrintTask : public sugar::TaskInterface {
public:
    void run() override {
        std::cout << "Hello from thread: " << std::this_thread::get_id() << std::endl;
    }
};

int main() {
    sugar::ThreadPool pool(sugar::WorkingType::Fixed, 4);
    for (int i = 0; i < 10; ++i) {
        pool.submit(std::make_shared<PrintTask>());
    }
    pool.shutdown();
    return 0;
}