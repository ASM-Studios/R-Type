#pragma once

#include <mutex>
#include <string>

class Logging {
private:
    static std::mutex _mutex;

    static std::string _getTimestamp();

public:
    static void info(std::string message);
    static void warn(std::string message);
    static void error(std::string message);
};
