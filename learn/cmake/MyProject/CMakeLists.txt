cmake_minimum_required(VERSION 3.10)

project(MyProject)

# 添加子目录（静态库和动态库）
add_subdirectory(mathlib)
add_subdirectory(util)

# 添加 include 目录，确保主程序可以找到头文件
include_directories(mathlib util)

# 生成主程序
add_executable(MyProject main.cpp)

# 链接静态库和动态库
target_link_libraries(MyProject mathlib util)
