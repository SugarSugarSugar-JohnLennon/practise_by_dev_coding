#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

int counter = 0;
mutex mtx;

void add_with_lock_guard() {
    // lock_guard 用于简单作用域加锁
    lock_guard<mutex> lock(mtx);
    cout << "[Thread 1] Acquired lock using lock_guard.\n";
    counter += 10;
    cout << "[Thread 1] Counter = " << counter << endl;
}

void add_with_unique_lock() {
    unique_lock<mutex> lock(mtx); // 可以更灵活控制锁的生命周期
    cout << "[Thread 2] Acquired lock using unique_lock.\n";
    if (counter < 50) {
        counter += 20;
        cout << "[Thread 2] Counter = " << counter << endl;
    }
    // lock 会在作用域结束时释放
}

int main() {
    thread t1(add_with_lock_guard);
    thread t2(add_with_unique_lock);

    // 主线程直接使用 lock/unlock（不推荐，但演示一下）
    this_thread::sleep_for(chrono::milliseconds(10)); // 保证其他线程有机会先执行
    mtx.lock();
    cout << "[Main] Acquired lock using raw lock/unlock.\n";
    counter += 5;
    cout << "[Main] Counter = " << counter << endl;
    mtx.unlock();

    t1.join();
    t2.join();

    cout << "Final Counter = " << counter << endl;
    return 0;
}
