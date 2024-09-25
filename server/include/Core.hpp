#pragma once

#include "Clock.hpp"
#include <span>

class Core {
private:
    int _tps;
    int _tickTime;
    Clock _tpsClock;
    bool _isRunning;
    int _port;
    std::string _hitboxes_config_file;

    void _waitTPS();

public:
    explicit Core();
    ~Core() = default;

    void init(const std::span<char *>& args);
    int run();
};
