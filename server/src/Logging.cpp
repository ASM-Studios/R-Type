#include "Logging.hpp"
#include <ctime>
#include <iostream>

std::string Logging::_getTimestamp() {
    struct tm *timeinfo = nullptr;
    time_t timestamp = time(nullptr);
    timeinfo = localtime(&timestamp);
    return std::format("{:02}:{:02}:{:02}", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

void Logging::info(std::string message) {
    std::lock_guard<std::mutex> lock(_mutex);
    std::cout << "[INF]"
              << std::format("{}$ {}", _getTimestamp(), message) << std::endl;
}

void Logging::warn(std::string message) {
    std::lock_guard<std::mutex> lock(_mutex);
    std::cout << "\033[33m" << "[WAR]" << "\033[0m"
              << std::format("{}$ {}", _getTimestamp(), message) << std::endl;
}

void Logging::error(std::string message) {
    std::lock_guard<std::mutex> lock(_mutex);
    std::cout << "\033[31m" << "[ERR]" << "\033[0m"
              << std::format("{}$ {}", _getTimestamp(), message) << std::endl;
}

std::mutex Logging::_mutex;
