#include <iostream>
#include <thread>

using namespace std;

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

/* out:
    this is thread 1x =  主线程
    x =  3 ,this is thread 2
    69 ,this is thread 2
    子线程结束.
 */