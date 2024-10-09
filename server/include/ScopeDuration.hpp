#pragma once

#include "Clock.hpp"

class ScopeDuration {
private:
    double _ms;
    Clock _clock;

public:
    explicit ScopeDuration(double ms) :
        _ms(ms) {}
    ~ScopeDuration();
};
