#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <iostream>

void my_coroutine(boost::asio::io_context& io, boost::asio::yield_context yield, int second) {
    boost::asio::steady_timer timer(io, boost::asio::chrono::seconds(second));

    std::cout << "Starting coroutine..." << std::endl;

    // 异步等待定时器，使用 yield 挂起协程
    timer.async_wait(yield);

    std::cout << "Coroutine Timer expired!" << std::endl;
}

void my_coroutine2(boost::asio::io_context& io, boost::asio::yield_context yield, int milliseconds) {
    std::cout << "Starting coroutine2..." << std::endl;

    // 异步等待定时器，使用 yield 挂起协程
    for (size_t i = 0; i < 30; i++) {
        boost::asio::steady_timer timer(io, boost::asio::chrono::milliseconds(milliseconds));
        timer.async_wait(yield);
        std::cout << "Coroutine2 Timer expired " << i+1 << "!" << std::endl;
    }
}

int main() {
    boost::asio::io_context io;

    int second = 5, milliseconds = 300;
    // 启动协程
    boost::asio::spawn(io, [=, &io](boost::asio::yield_context yield) {
        my_coroutine(io, yield, second);
    });
    boost::asio::spawn(io, [=, &io](boost::asio::yield_context yield) {
        my_coroutine2(io, yield, milliseconds);
    });

    // 运行 io_context
    io.run();

    std::cout << "Hello world!\n";

    return 0;
}