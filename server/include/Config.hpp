#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <libconfig.h++>

constexpr auto DEFAULT_CONFIG = "server/config.json";

class Config {
private:
    boost::property_tree::ptree pt;
    std::unordered_map<std::string, std::string> data;
    static std::unique_ptr<Config> instance;
    static std::mutex mutex;

    explicit Config(const std::string& filePath);

public:
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    static Config& getInstance(const std::string& filePath);
    std::optional<std::string> get(const std::string& field) const;
};
