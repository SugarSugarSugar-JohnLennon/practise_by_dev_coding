# 多线程

## 创建线程
创建线程很简单，只需要把函数添加到线程当中即可。
type 1
```cpp
std::thread myThread ( thread_fun);
//函数形式为void thread_fun()
myThread.join();
//同一个函数可以代码复用，创建多个线程
```

type 2
```cpp
std::thread myThread ( thread_fun(100));
myThread.join();
//函数形式为void thread_fun(int x)
//同一个函数可以代码复用，创建多个线程
```

type3
```cpp
std::thread (thread_fun,1).detach();
//直接创建线程，没有名字
//函数形式为void thread_fun(int x)
```
example:      [to_create_thread.cpp](to_create_thread.cpp)

---
```cpp
void thread_1() {
    cout << "this is thread 1" << endl;
}

void thread_2(int x) {
    cout << "x =  "<< x << " ,this is thread 2" << endl;
}

int main() {
    thread th1(thread_1);      //  开启线程1，调用：thread_1()
    thread th2(thread_2, 69);  //  开启线程2，调用：thread_2() 传入x
    thread th3(thread_2,3);    //  开启线程3，共享thread_2函数。

    cout << "主线程\n";
    th1.join();
    th2.join();
    th3.join();
    cout << "子线程结束.\n";//必须join完成
    return 0;
}
```
输出：
```SHELL
this is thread 1x =  主线程

x =  3 ,this is thread 2
69 ,this is thread 2
子线程结束.
```
这是正常的行为。因为：

所有的 cout 输出都写到标准输出（stdout）；

多个线程同时往 stdout 写内容，但 cout 不是线程安全的；

所以输出可能会交叉/重叠。


## join vs detach
当线程启动后，一定要在和线程相关联的thread销毁前，确定以何种方式等待线程执行结束。比如上例中的join。

- detach方式，启动的线程自主在后台运行，当前的代码继续往下执行，不等待新线程结束。
- join方式，等待启动的线程完成，才会继续往下执行。

可以使用joinable判断是join模式还是detach模式:
```cpp
if (myThread.joinable()) foo.join();
```

## this_thread
this_thread是一个类，它有4个功能函数，具体如下：
![](https://i0.hdslb.com/bfs/new_dyn/f826ca100d093104e2627480d690628b29529286.jpg@674w_228h_1e_1c.webp)

```cpp
using std::chrono::system_clock;
std::time_t tt = system_clock::to_time_t(system_clock::now());
struct std::tm * ptm = std::localtime(&tt);
cout << "Waiting for the next minute to begin...\n";
++ptm->tm_min; //加一分钟
ptm->tm_sec = 0; //秒数设置为0//暂停执行，到下一整分执行
this_thread::sleep_until(system_clock::from_time_t(mktime(ptm)));
```

## Mutex
![](https://i0.hdslb.com/bfs/new_dyn/13c13ac10917a3ce0290b0f467549b2129529286.jpg@1052w_!web-dynamic.avif)
std::mutex 是C++11 中最基本的互斥量，std::mutex 对象提供了独占所有权的特性——即不支持递归地对 std::mutex 对象上锁，而 std::recursive_lock 则可以递归地对互斥量对象上锁。

### lock与unlock
mutex常用操作：

- lock()：资源上锁
- unlock()：解锁资源
- trylock()：查看是否上锁，它有下列3种类情况：
    1. 未上锁返回false，并锁住；
    2. 其他线程已经上锁，返回true；
    3. 同一个线程已经对它上锁，将会产生死锁。

死锁：是指两个或两个以上的进程在执行过程中，由于竞争资源或者由于彼此通信而造成的一种阻塞的现象，若无外力作用，它们都将无法推进下去。此时称系统处于死锁状态或系统产生了死锁，这些永远在互相等待的进程称为死锁进程。
```cpp
std::mutex mtx;  
mtx.lock();   // 上锁了
//  do something...   
mtx.unlock(); // 解锁
```

### lock_guard
创建lock_guard对象时，它将尝试获取提供给它的互斥锁的所有权。当控制流离开lock_guard对象的作用域时，lock_guard析构并释放互斥量。lock_guard的特点：
- 创建即加锁，作用域结束自动析构并解锁，无需手工解锁
- 不能中途解锁，必须等作用域结束才解锁
- 不能复制

```CPP
std::mutex mtx;  

{
    const std::lock_guard<std::mutex> lock(mtx); // 上锁
} // 自动解锁

```

### unique_lock
简单地讲，unique_lock 是 lock_guard 的升级加强版，它具有 lock_guard 的所有功能，同时又具有其他很多方法，使用起来更加灵活方便，能够应对更复杂的锁定需要。unique_lock的特点：
- 创建时可以不锁定（通过指定第二个参数为std::defer_lock），而在需要时再锁定
- 可以随时加锁解锁
- 作用域规则同 lock_grard，析构时自动释放锁
- 不可复制，可移动
- 条件变量需要该类型的锁作为参数（此时必须使用unique_lock）


所有 lock_guard 能够做到的事情，都可以使用 unique_lock 做到，反之则不然。

使用lock_guard的情况：需要使用锁的时候，首先考虑使用 lock_guard，因为lock_guard是最简单的锁。

examples: [test_mutex_and_lock_unique_guard.cpp](test_mutex_and_lock_unique_guard.cpp)

## condition_variable 条件变量
wait to be done...
## 线程池
wait to be done...
---