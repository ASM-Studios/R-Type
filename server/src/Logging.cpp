#include "Logging.hpp"
#include <ctime>
#include <iostream>
#include <format>

std::string Logging::_getTimestamp() {
    struct tm *timeinfo = nullptr;
    time_t timestamp = time(nullptr);
    timeinfo = localtime(&timestamp);
    return std::format("{:02}:{:02}:{:02}", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

void Logging::debug(const std::string& message) {
    if (_logLevel < DEBUG) {
        return;
    }
    std::lock_guard<std::mutex> lock(_mutex);
    std::cout << std::format("\033[32m[DEBUG {}]\033[0m: {}", _getTimestamp(), message) << std::endl;
}

void Logging::info(const std::string& message) {
    if (_logLevel < INFO) {
        return;
    }
    std::lock_guard<std::mutex> lock(_mutex);
    std::cout << std::format("\033[34m[INFO {}]\033[0m: {}", _getTimestamp(), message) << std::endl;
}

void Logging::warn(const std::string& message) {
    if (_logLevel < WARN) {
        return;
    }
    std::lock_guard<std::mutex> lock(_mutex);
    std::cout << std::format("\033[33m[WARN {}]\033[0m: {}", _getTimestamp(), message) << std::endl;
}

void Logging::error(const std::string& message) {
    if (_logLevel < ERROR) {
        return;
    }
    std::lock_guard<std::mutex> lock(_mutex);
    std::cout << std::format("\033[31m[ERROR {}]\033[0m: {}", _getTimestamp(), message) << std::endl;
}

void Logging::setLogLevel(const Logging::Level& level) {
    _logLevel = level;
}

Logging::Level Logging::_logLevel(Logging::Level::ERROR);
std::mutex Logging::_mutex;
