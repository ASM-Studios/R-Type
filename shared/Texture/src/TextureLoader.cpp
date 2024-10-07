#include "TextureLoader.hpp"
#include "Logger.hpp"
#include <format>
#include <libconfig.h++>
#include <mutex>
#include <string>

TextureLoader& TextureLoader::getInstance() {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_instance == nullptr) {
        _instance = std::unique_ptr<TextureLoader>(new TextureLoader());
    }
    return *_instance;
}

void TextureLoader::loadFile(const std::string& path) {
    this->_cfg.readFile(path);
}

void TextureLoader::loadTexture(const std::string& path, Type type) {
    const libconfig::Setting& element = this->_cfg.lookup(path);
    int id = 0;
    int row = 1;
    int col = 1;
    float scale = 1.0;
    element.lookupValue("id", id);
    element.lookupValue("row", row);
    element.lookupValue("col", col);
    element.lookupValue("scale", scale);
    this->_textures.insert({id, {type, Texture(element.getPath(), row, col, scale)}});
}

void TextureLoader::loadTextures(std::string path, Type type) {
    libconfig::Setting& section = this->_cfg.lookup(path);
    int id = 0;
    for (auto& texture: section) {
        if (!texture.exists("id")) {
            Logger::log(LogLevel::WARNING, std::format("Failed to retrieve id of texture {0}", texture.getName()));
            continue;
        }
        this->loadTexture(texture.getPath(), type);
    }
}

Texture& TextureLoader::getTexture(const int id) {
    return this->_textures.at(id).second;
}

std::size_t TextureLoader::getNoTexture() const {
    return this->_textures.size();
}

std::unique_ptr<TextureLoader> TextureLoader::_instance(nullptr);
std::mutex TextureLoader::_mutex;
