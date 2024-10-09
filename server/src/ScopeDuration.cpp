#include "ScopeDuration.hpp"
#include <thread>

ScopeDuration::~ScopeDuration() {
    double elapsed = this->_clock.get();
    if (elapsed < this->_ms) {
        int time = static_cast<int>((this->_ms - elapsed) * 1000);
        std::this_thread::sleep_for(std::chrono::microseconds(time));
    }
}
