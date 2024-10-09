#include "Clock.hpp"
#include <chrono>

Clock::Clock() :
    _start(std::chrono::system_clock::now()),
    _end(std::chrono::system_clock::now()) {}

void Clock::reset() {
    this->_start = std::chrono::system_clock::now();
    this->_end = std::chrono::system_clock::now();
}

double Clock::get() {
    this->_end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedTime = this->_end - this->_start;

    return elapsedTime.count() * 1000;
}
