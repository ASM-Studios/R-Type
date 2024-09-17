#pragma once

#include <iostream>
#include <string>

#include "Config.hpp"

enum class LogLevel {
    NONE = -1,
    INFO = 0,
    WARNING = 1,
    ERROR = 2
};

class Logger {
    private:
        static LogLevel getLevel(const std::string& level) {
            if (level == "INFO") {
                return LogLevel::INFO;
            } else if (level == "WARNING") {
                return LogLevel::WARNING;
            } else if (level == "ERROR") {
                return LogLevel::ERROR;
            }
            return LogLevel::NONE;
        }

    public:
        static void log(const LogLevel logLevel, const std::string& message) {
            Config& config = Config::getInstance("client/config.json");
            std::string configLogLevel = config.get("logLevel").value_or("NONE");
            if (logLevel <= getLevel(configLogLevel)) {
                switch (logLevel) {
                case LogLevel::INFO:
                    std::cout << "\033[32m[INFO] " << message << "\033[0m" << std::endl;
                    break;
                case LogLevel::WARNING:
                    std::cout << "\033[33m[WARNING] " << message << "\033[0m" << std::endl;
                    break;
                case LogLevel::ERROR:
                    std::cerr << "\033[31m[ERROR] " << message << "\033[0m" << std::endl;
                    break;
                default:
                    break;
                }
            }
        }
};
