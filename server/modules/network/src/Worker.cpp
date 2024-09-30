#include "Worker.hpp"
#include "Logger.hpp"
#include "Query.hpp"
#include <chrono>
#include <future>

void execute(std::reference_wrapper<std::promise<bool>> promise, std::pair<network::Client, Query> query) {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    Logger::log(LogLevel::INFO, std::format("Result: {0}", query.second.req));
    promise.get().set_value(true);
}

namespace network {
    Worker::Worker(std::pair<Client, Query> query) :
        _future(this->_promise.get_future()),
        _thread(&execute, std::ref(this->_promise), query) {}

    void Worker::join() {
        this->_thread.join();
    }

    bool Worker::isReady() {
        auto status = this->_future.wait_for(std::chrono::milliseconds(0));
        return status == std::future_status::ready;
    }
}
