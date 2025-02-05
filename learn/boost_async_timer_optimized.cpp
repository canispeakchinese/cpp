#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <iostream>
#include <chrono>

boost::asio::awaitable<void> async_timer(boost::asio::io_context& io_context, int seconds) {
    // 创建一个定时器
    boost::asio::steady_timer timer(io_context, std::chrono::seconds(seconds));
    
    // 等待定时器到期
    co_await timer.async_wait(boost::asio::use_awaitable);
    
    std::cout << "Timer expired after " << seconds << " seconds!" << std::endl;
}

int main() {
    boost::asio::io_context io_context;

    // 启动协程，定时器等待 3 秒后打印消息
    boost::asio::co_spawn(io_context, async_timer(io_context, 3), boost::asio::detached);

    // 启动协程，定时器等待 1 秒后打印消息
    boost::asio::co_spawn(io_context, async_timer(io_context, 1), boost::asio::detached);

    // 启动协程，定时器等待 2 秒后打印消息
    boost::asio::co_spawn(io_context, async_timer(io_context, 2), boost::asio::detached);

    io_context.run();  // 运行 io_context
    return 0;
}
