#pragma once

#include "Client.hpp"
#include "query/RawRequest.hpp"
#include <future>
#include <thread>

namespace network {
    class Worker {
        private:
            std::promise<bool> _promise;
            std::future<bool> _future;
            std::thread _thread;

        public:
            explicit Worker(std::pair<Client, RawRequest> query);
            ~Worker() = default;

            void join();
            bool isReady();
    };
}
