#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include <string>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

// 处理HTTP请求的协程
void handle_request(asio::yield_context yield, tcp::socket& socket)
{
    beast::flat_buffer buffer;
    http::request<http::string_body> request;

    // 读取HTTP请求
    http::async_read(socket, buffer, request, yield);

    // 创建一个HTTP响应，内容与请求相同
    http::response<http::string_body> response{http::status::ok, request.version()};
    response.body() = request.body();
    response.prepare_payload();

    // 发送HTTP响应
    http::async_write(socket, response, yield);
}

// 接受连接的协程
void accept_connections(asio::yield_context yield, tcp::acceptor& acceptor)
{
    while (true)
    {
        auto socket = std::make_shared<tcp::socket>(acceptor.get_executor());
        acceptor.async_accept(*socket, yield);

        // 为每个连接启动一个协程来处理请求
        asio::spawn(acceptor.get_executor(), [socket](asio::yield_context yield)
        {
            try
            {
                handle_request(yield, *socket);
            }
            catch (std::exception& e)
            {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        });
    }
}

int main()
{
    try
    {
        asio::io_context io_context;

        // 监听8080端口
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

        // 启动协程来接受连接
        asio::spawn(io_context, [&](asio::yield_context yield)
        {
            accept_connections(yield, acceptor);
        });

        // 运行IO上下文
        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}