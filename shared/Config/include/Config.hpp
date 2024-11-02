#pragma once

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <libconfig.h++>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

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

static inline std::pair<std::size_t, std::size_t> getResolution() {
    auto& config = Config::getInstance("config/client.json");
    int width = std::stoi(config.get("width").value_or("1920"));
    int height = std::stoi(config.get("height").value_or("1080"));
    return {width, height};
}
