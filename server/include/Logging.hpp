#pragma once

#include <string>

class Logging {
private:
    static std::string _getTimestamp();

public:
    static void info(std::string message);
    static void warn(std::string message);
    static void error(std::string message);
};
