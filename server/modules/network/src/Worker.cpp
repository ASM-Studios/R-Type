#include "Worker.hpp"
#include "Logging.hpp"
#include "Query.hpp"
#include <chrono>
#include <future>

void execute(std::reference_wrapper<std::promise<bool>> promise, Query query) {
    sleep(5);
    Logging::info(std::string("Result:") + query.req);
    promise.get().set_value(true);
}

namespace network {
    Worker::Worker(Query query) :
        _future(this->_promise.get_future()),
        _thread(std::make_shared<std::thread>(&execute, std::ref(this->_promise), query)) {}

    void Worker::join() {
        this->_thread->join();
    }

    bool Worker::isReady() {
        auto status = this->_future.wait_for(std::chrono::milliseconds(0));
        return status == std::future_status::ready;
    }
}
