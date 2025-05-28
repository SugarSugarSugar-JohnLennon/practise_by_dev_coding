#include <iostream>
#include <functional> 

class Button {
public:
    // 定义回调类型（接收无参数、无返回值的函数）
    using Callback = std::function<void()>;

    // 注册回调函数
    void setCallback(Callback cb) {
        callback = cb;
        std::cout << "回调函数已注册！" << std::endl;
    }

    // 模拟点击按钮
    void click() {
        std::cout << "按钮被点击！" << std::endl;
        if (callback) {
            callback();  // 调用回调
        } else {
            std::cout << "没有设置回调函数！" << std::endl;
        }
    }

private:
    Callback callback; // 存储回调函数
};

// 用户定义的回调逻辑
void onButtonClicked() {
    std::cout << "执行用户定义的回调逻辑：按钮被处理。" << std::endl;
}


int main() {
    Button btn;
    btn.setCallback(onButtonClicked); // 注册回调函数
    btn.click();                       // 模拟点击，触发回调
    
    return 0;
}
