#pragma once

#include <SFML/Graphics.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <unordered_map>
#include <string>
#include <memory>

#include "Config.hpp"
#include "GuiException.hpp"

namespace GUI {
    constexpr auto DEFAULT_SPRITES_CONFIG = "assets/texture_config.cfg";

    class SpriteManager : public std::enable_shared_from_this<SpriteManager> {
        private:
            std::size_t _width;
            std::size_t _height;

            std::unordered_map<std::string, std::shared_ptr<sf::Texture>> _textures;
            std::unordered_map<std::string, std::map<std::size_t, std::shared_ptr<sf::Sprite>>> _sprites;

            void importSprites(const std::string& spritePath, const std::string& name);
            void loopDirectory(const std::string &spritePath, const std::string &relativePath);

            std::string _spritesConfigPath;

        public:
            SpriteManager() = default;
            ~SpriteManager();
            void init();

            void updateWindowSize(const std::size_t width, const std::size_t height) {
                _width = width;
                _height = height;
            }

            std::shared_ptr<sf::Texture> getTexture(const std::string& name) const {
                if (const auto it = _textures.find(name); it != _textures.end()) {
                    return it->second;
                }
                Logger::log(LogLevel::ERR, "Texture not found for name: " + name);
                return {};
            }

            std::shared_ptr<sf::Sprite> getSprite(const std::string& name, const std::size_t id) const {
                if (const auto it = _sprites.find(name); it != _sprites.end()) {
                    if (const auto spriteIt = it->second.find(id); spriteIt != it->second.end()) {
                        return spriteIt->second;
                    }
                    Logger::log(LogLevel::ERR, "Sprite ID not found for name: " + name);
                    return {};
                }
                Logger::log(LogLevel::ERR, "Sprite not found for name: " + name + " and id: " + std::to_string(id));
                return {};
            }

            std::map<std::size_t, std::shared_ptr<sf::Sprite>> getSprites(const std::string& name) const {
                if (const auto it = _sprites.find(name); it != _sprites.end()) {
                    return it->second;
                }
                Logger::log(LogLevel::ERR, "Sprite not found for name: " + name);
                return {};
            }
    };
};
