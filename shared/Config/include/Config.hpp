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
