#pragma once

#include <atomic>
#include <mutex>

class StatusMutex {
    private:
        std::mutex _mutex;
        std::atomic<bool> _isLocked;

    public:
        explicit StatusMutex() : _isLocked(false) {}

        void lock() {
            this->_isLocked = true;
            this->_mutex.lock();
        }

        void unlock() {
            this->_isLocked = false;
            this->_mutex.unlock();
        }

        bool isLocked() {
            return this->_isLocked;
        }
};
