#pragma once

#include "Query.hpp"
#include <future>
#include <memory>
#include <thread>

namespace network {
    class Worker {
    private:
        std::promise<bool> _promise;
        std::future<bool> _future;
        std::shared_ptr<std::thread> _thread;

    public:
        explicit Worker(Query query);
        ~Worker() = default;

        void join();
        bool isReady();
    };
}
