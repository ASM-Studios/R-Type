#pragma once

#include <atomic>
#include <mutex>

class StatusMutex {
    private:
        std::mutex _mutex;
        std::atomic<bool> _isLocked;

    public:
        explicit StatusMutex() :
            _isLocked(false) {}

        void lock() {
            this->_mutex.lock();
            this->_isLocked = true;
        }

        void unlock() {
            this->_mutex.unlock();
            this->_isLocked = false;
        }

        bool isLocked() {
            return this->_isLocked;
        }
};
