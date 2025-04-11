#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>
#include <string>

class JobManager {
public:
    JobManager(boost::asio::io_context& io_context)
        : io_context_(io_context) {}

    // 添加一个任务，包括准备阶段、运行阶段和加载结果阶段
    template <typename PrepareFunc, typename RunFunc, typename LoadFunc>
    void add_job(const std::string& name, PrepareFunc&& prepare, RunFunc&& run, LoadFunc&& load) {
        pending_jobs_[name] = {std::forward<PrepareFunc>(prepare), 
                               std::forward<RunFunc>(run), 
                               std::forward<LoadFunc>(load)};
        
        // 直接执行准备阶段
        boost::asio::co_spawn(io_context_, prepare(name), boost::asio::detached);
        // 执行完成之后执行运行阶段
        boost::asio::co_spawn(io_context_, run(name), boost::asio::detached);
    }

    void wait() {
        io_context_.run();
    }

    boost::asio::io_context& io_context_;

    // 任务信息
    struct Job {
        std::function<boost::asio::awaitable<void>(const std::string&)> prepare;
        std::function<boost::asio::awaitable<void>(const std::string&)> run;
        std::function<boost::asio::awaitable<void>(const std::string&)> load;
    };

    // 存储所有待执行任务的详细信息
    std::unordered_map<std::string, Job> pending_jobs_;
};

boost::asio::awaitable<void> prepare_param(boost::asio::io_context& io_context, const std::string& name, JobManager& manager) {
    std::cout << name << " preparing params..." << std::endl;

    // 如果任务需要依赖其他任务的结果，则在准备阶段调用它们的load方法来获取结果
    if (name == "Job2") {
        // Job2 需要 Job1 的结果
        std::cout << "Job 2 preparing param, loading Job 1's result..." << std::endl;
        co_await manager.pending_jobs_["Job1"].load("Job1");  // 在 prepare 阶段调用 Job1 的 load 函数来获取结果
    }

    // 模拟准备阶段完成
    co_return;
}

boost::asio::awaitable<void> run_job(boost::asio::io_context& io_context, const std::string& name) {
    std::cout << name << " running..." << std::endl;
    co_return;
}

boost::asio::awaitable<void> load_result(boost::asio::io_context& io_context, const std::string& name) {
    std::cout << name << " loading result..." << std::endl;
    co_return;
}

int main() {
    boost::asio::io_context io_context;

    // 创建 JobManager
    JobManager job_manager(io_context);

    // 创建任务，任务 Job1 不依赖任何任务，任务 Job2 依赖任务 Job1
    job_manager.add_job(
        "Job1",  // 名字为 Job1
        [&](const std::string& name) { return prepare_param(io_context, name, job_manager); },
        [&](const std::string& name) { return run_job(io_context, name); },
        [&](const std::string& name) { return load_result(io_context, name); }
    );  // Job 1

    job_manager.add_job(
        "Job2",  // 名字为 Job2
        [&](const std::string& name) { return prepare_param(io_context, name, job_manager); },
        [&](const std::string& name) { return run_job(io_context, name); },
        [&](const std::string& name) { return load_result(io_context, name); }
    );  // Job 2

    job_manager.wait();

    std::cout << "All jobs completed!" << std::endl;

    return 0;
}
