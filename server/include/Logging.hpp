#pragma once

#include <mutex>
#include <string>

class Logging {
public:
    enum Level {
        ERR,
        WARN,
        INFO,
        DEBUG,
        ALL
    };

private:
    static Level _logLevel;
    static std::mutex _mutex;

    static std::string _getTimestamp();

public:
    static void debug(const std::string& message);
    static void info(const std::string& message);
    static void warn(const std::string& message);
    static void error(const std::string& message);

    static void setLogLevel(const Logging::Level& level);
};
