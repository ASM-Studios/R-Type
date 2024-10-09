#include "Worker.hpp"
#include "Logger.hpp"
#include "query/Header.hpp"
#include "query/NonTypedQuery.hpp"
#include "query/RawRequest.hpp"
#include "query/TypedQuery.hpp"
#include <chrono>
#include <future>

static void callCallback(std::pair<network::Client, RawRequest> request) {
    auto it = requestAction.find(request.second.getQuery().getHeader().requestType);
    if (it != requestAction.end()) {
        it->second(request.first, request.second);
    } else {
        Logger::log(LogLevel::ERR, "Callback not found");
    }
}

void execute(std::reference_wrapper<std::promise<bool>> promise, std::pair<network::Client, RawRequest> request) {
    if (!request.second.verifChecksum()) {
        Logger::log(LogLevel::ERR, "Checksum is invalid");
    } else {
        callCallback(request);
    }
    promise.get().set_value(true);
}

namespace network {
    Worker::Worker(std::pair<Client, RawRequest> query) :
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
