#pragma once

#include "Client.hpp"
#include "Query.hpp"
#include <future>
#include <memory>
#include <thread>

namespace network {
    class Worker {
    private:
        std::promise<bool> _promise;
        std::future<bool> _future;
        std::thread _thread;

    public:
        explicit Worker(std::pair<Client, Query> query);
        ~Worker() = default;

        void join();
        bool isReady();
    };
}
