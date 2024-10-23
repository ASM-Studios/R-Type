#include "ThreadPool.hpp"

ThreadPool::ThreadPool(size_t nbThreads) : _stop(false) {
    for (size_t i = 0; i < nbThreads; ++i) {
        _workers.emplace_back([this] {
            for (;;) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(_mutex);
                    _condition.wait(lock, [this] { return _stop || !_tasks.empty(); });
                    if (_stop && _tasks.empty()) return;
                    task = std::move(_tasks.front());
                    _tasks.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _stop = true;
    }
    _condition.notify_all();
    for (std::thread &worker : _workers) worker.join();
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _tasks.emplace(std::move(task));
    }
    _condition.notify_one();
}