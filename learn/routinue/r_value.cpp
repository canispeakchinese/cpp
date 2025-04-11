#include <iostream>
#include <vector>
#include <string>

class MyClass {
public:
    int data;

    MyClass(int str) : data(std::move(str)) {
        std::cout << "Constructor: " << data << std::endl;
    }

    // 移动构造函数
    MyClass(MyClass&& other) noexcept {
        data = std::move(other.data);  // 将资源从临时对象转移过来
        std::cout << "Move Constructor: " << data << std::endl;
    }

    // 移动赋值操作符
    MyClass& operator=(MyClass&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);  // 转移资源
            std::cout << "Move Assignment: " << data << std::endl;
        }
        return *this;
    }

    ~MyClass() {
        std::cout << "Destructor: " << data << std::endl;
    }
};

int main() {
    MyClass a(3);
    MyClass b = std::move(a); // 使用右值引用进行资源转移
}