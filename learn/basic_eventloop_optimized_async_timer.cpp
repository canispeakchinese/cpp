#include <iostream>
#include <chrono>
#include <queue>
#include <functional>
#include <atomic>
#include <thread>

using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;
using Duration = Clock::duration;

// 定时器事件结构
struct TimerEvent {
    TimePoint time;
    std::function<void()> callback;

    bool operator<(const TimerEvent& other) const {
        return time > other.time; // 小顶堆
    }
};

// 全局事件队列
std::priority_queue<TimerEvent> event_queue;
std::atomic<bool> running{true};

// 添加定时器
void add_timer(Duration duration, std::function<void()> callback) {
    TimePoint trigger_time = Clock::now() + duration;
    event_queue.push({trigger_time, std::move(callback)});
}

// 非阻塞事件循环
void run_event_loop() {
    while (running) {
        if (!event_queue.empty()) {
            TimePoint now = Clock::now();
            TimerEvent event = event_queue.top();

            if (now >= event.time) {
                event_queue.pop();
                event.callback(); // 触发回调
            } else {
                // 动态调整轮询频率
                auto time_until_event = event.time - now;
                if (time_until_event > std::chrono::milliseconds(10)) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                } else {
                    std::this_thread::yield();
                }
            }
        } else {
            // 没有定时器事件时，降低轮询频率
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

int main() {
    add_timer(std::chrono::seconds(2), []() {
        std::cout << "Timer 2s expired!" << std::endl;
    });

    add_timer(std::chrono::seconds(1), []() {
        std::cout << "Timer 1s expired!" << std::endl;
    });

    add_timer(std::chrono::milliseconds(1010), []() {
        std::cout << "Timer 1.01s expired!" << std::endl;
    });

    run_event_loop(); // 运行事件循环

    return 0;
}