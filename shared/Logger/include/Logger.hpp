#pragma once

#include "Config.hpp"
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

enum class LogLevel {
    NONE = -1,
    INFO = 2,
    WARNING = 1,
    ERR = 0
};

class Logger {
    private:
        static std::mutex _mutex;
        static LogLevel getLevel(const std::string& level) {
            if (level == "INFO") {
                return LogLevel::INFO;
            } else if (level == "WARNING") {
                return LogLevel::WARNING;
            } else if (level == "ERROR") {
                return LogLevel::ERR;
            }
            return LogLevel::NONE;
        }

    public:
        static void log(const LogLevel logLevel, const std::string& message) {
            std::lock_guard<std::mutex> lock(_mutex);
            Config& config = Config::getInstance("server/config.json");
            if (const std::string configLogLevel = config.get("logLevel").value_or("NONE"); logLevel <= getLevel(configLogLevel)) {
                const auto now = std::chrono::system_clock::now();
                const std::time_t now_time = std::chrono::system_clock::to_time_t(now);
                const std::tm *now_tm = std::localtime(&now_time);

                std::ostringstream timeStream;
                timeStream << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");

                switch (logLevel) {
                    case LogLevel::INFO:
                        std::cout << "\033[32m[INFO " << timeStream.str() << "] " << message << "\033[0m" << std::endl;
                        break;
                    case LogLevel::WARNING:
                        std::cout << "\033[33m[WARNING " << timeStream.str() << "] " << message << "\033[0m" << std::endl;
                        break;
                    case LogLevel::ERR:
                        std::cerr << "\033[31m[ERROR " << timeStream.str() << "] " << message << "\033[0m" << std::endl;
                        break;
                    default:
                        break;
                }
            }
        }
};
