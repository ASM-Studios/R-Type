#pragma once

#include "Mutex.hpp"
#include <memory>
#include <mutex>
#include <optional>

template <typename T>
class Singleton {
    private:
        static std::unique_ptr<Singleton> _instance;
        static std::mutex _mutex;

        std::atomic<bool> _isSafe;
        StatusMutex _accessMutex;

        std::optional<T> _data;

        explicit Singleton() :
            _isSafe(true),
            _data(std::nullopt) {}

    public:
        ~Singleton() = default;
        Singleton(const Singleton& other) = delete;

        Singleton& operator=(const Singleton& other) = delete;

        static Singleton& getInstance();

        template <typename... Args>
        static Singleton<T>& wrap(Args... args);
        static Singleton<T>& wrap(T& data);

        void lock() {
            this->_accessMutex.lock();
        }

        void unlock() {
            this->_accessMutex.unlock();
        }

        void setSafe(bool safe) {
            this->_isSafe = safe;
        }

        T& get();
        bool has_value();
};

#include "Singleton.inl"
