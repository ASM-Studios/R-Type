#include "Config.hpp"

std::unique_ptr<Config> Config::instance = nullptr;
std::mutex Config::mutex;

Config::Config(const std::string& filePath) {
    try {
        boost::property_tree::read_json(filePath, pt);
        for (const auto& [fst, snd]: pt) {
            data[fst] = snd.data();
        }
    } catch (const std::exception& e) {
        std::cerr << "Unable to open or parse config file: " << e.what() << std::endl;
    }
}

Config& Config::getInstance(const std::string& filePath) {
    std::lock_guard<std::mutex> const lock(mutex);
    if (!instance) {
        instance.reset(new Config(filePath));
    }
    return *instance;
}

std::optional<std::string> Config::get(const std::string& field) const {
    if (const auto it = data.find(field); it != data.end()) {
        return it->second;
    }
    return std::nullopt;
}
