#include <iostream>
#include <mutex>

class Singleton {
public:
    static Singleton& getInstance() {
        if (instance == nullptr) {
            std::lock_guard<std::mutex> lock(mutex);  
            if (instance == nullptr) {
                instance = new Singleton();
            }
        }
        return *instance;
    }

private:
    Singleton() {
        std::cout << "Singleton instance created." << std::endl;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static Singleton* instance;
    static std::mutex mutex;  
};
Singleton* Singleton::instance = nullptr;
std::mutex Singleton::mutex;


