#pragma once

#include <chrono>

class Clock {
    private:
        std::chrono::time_point<std::chrono::system_clock> _start;
        std::chrono::time_point<std::chrono::system_clock> _end;

    public:
        explicit Clock();

        void reset();
        double get();
};
