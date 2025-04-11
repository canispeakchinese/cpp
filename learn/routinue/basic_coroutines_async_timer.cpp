#include <iostream>
#include <chrono>
#include <thread>
#include <coroutine>
#include <functional>

// 定义一个简单的 Awaitable 类型，用于实现异步定时器
class AsyncTimer {
public:
    AsyncTimer(std::chrono::milliseconds duration) : duration_(duration) {}

    // 实现 await_ready：检查是否已经完成
    bool await_ready() const noexcept {
        return false; // 总是挂起，等待定时器完成
    }

    // 实现 await_suspend：挂起协程，启动定时器
    void await_suspend(std::coroutine_handle<> handle) noexcept {
        // 启动一个线程来模拟异步定时器
        std::thread([this, handle]() {
            std::this_thread::sleep_for(duration_); // 等待指定时间
            handle.resume(); // 恢复协程
        }).detach();
    }

    // 实现 await_resume：协程恢复时调用
    void await_resume() const noexcept {
        std::cout << "Timer expired!" << std::endl;
    }

private:
    std::chrono::milliseconds duration_;
};

// 定义一个协程函数
struct Coroutine {
    struct promise_type {
        Coroutine get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};

// 使用协程的异步定时器
Coroutine async_timer_example() {
    std::cout << "Starting timer..." << std::endl;

    // 使用 co_await 等待异步定时器
    co_await AsyncTimer(std::chrono::seconds(2));

    std::cout << "Timer finished!" << std::endl;
}

int main() {
    // 启动协程
    async_timer_example();

    // 主线程继续执行其他任务
    std::cout << "Main thread is running..." << std::endl;

    // 等待足够的时间，确保协程完成
    std::this_thread::sleep_for(std::chrono::seconds(3));

    return 0;
}