#include <iostream>
#include "mathlib.h"  // 静态库
#include "util.h"     // 动态库

int main() {
    int a = 5, b = 3;
    
    std::cout << "Addition: " << add(a, b) << std::endl;
    std::cout << "Multiplication: " << multiply(a, b) << std::endl;
    
    print_message("Hello from dynamic library!");
    
    return 0;
}
