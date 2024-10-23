#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool {
    public:
    ThreadPool(size_t nbThreads);
    ~ThreadPool();

    void enqueue(std::function<void()> task);

    private:
    std::vector<std::thread> _workers;
    std::queue<std::function<void()>> _tasks;

    std::mutex _mutex;
    std::condition_variable _condition;
    bool _stop;
};