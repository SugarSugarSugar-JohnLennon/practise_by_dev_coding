#include <iostream>
#include <thread>
using namespace std;
void thread_1() {
  int i = 0; 
  while(i < 9){
    cout<<"子线程1: " << i <<endl;
    ++i;
  }
}

void thread_2() {
  int i = 0; 
  while(i < 9){
    cout<<"子线程2: " << i <<endl;
    ++i;
  }
}

int main() {
    thread first ( thread_1); 
    thread second (thread_2); 

    first.detach(); 
    second.join(); 

    int i = 0; 
    while(i < 9){
      std::cout << "主线程: " << i << endl;
      ++i;
    }

    return 0;
}