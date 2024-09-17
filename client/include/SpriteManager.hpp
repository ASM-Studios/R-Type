#pragma once

#include <SFML/Graphics.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <unordered_map>
#include <string>
#include <memory>

#include "Config.hpp"
#include "GuiException.hpp"
#include "Logger.hpp"

namespace GUI {
    constexpr auto SPRITES_CONFIG = "client/assets/sprites_config.cfg";

    class SpriteManager : public std::enable_shared_from_this<SpriteManager> {
        private:
            std::unordered_map<std::string, std::shared_ptr<sf::Texture>> _textures;
            std::unordered_map<std::string, std::map<std::size_t, std::shared_ptr<sf::Sprite> >> _sprites;

            void importSprites(const std::string& spritePath, const std::string& name);

        public:
            SpriteManager();
            ~SpriteManager();

            std::shared_ptr<sf::Texture> getTexture(const std::string& name) const {
                if (const auto it = _textures.find(name); it != _textures.end()) {
                    return it->second;
                }
                Logger::log(LogLevel::ERROR, "Texture not found for name: " + name);
                return {};
            }

            std::shared_ptr<sf::Sprite> getSprite(const std::string& name, const std::size_t id) const {
                if (const auto it = _sprites.find(name); it != _sprites.end()) {
                    if (const auto spriteIt = it->second.find(id); spriteIt != it->second.end()) {
                        return spriteIt->second;
                    }
                    Logger::log(LogLevel::ERROR, "Sprite ID not found for name: " + name);
                    return {};
                }
                Logger::log(LogLevel::ERROR, "Texture not found for name: " + name);
                return {};
            }

            std::map<std::size_t, std::shared_ptr<sf::Sprite>> getSprites(const std::string& name) const {
                if (const auto it = _sprites.find(name); it != _sprites.end()) {
                    return it->second;
                }
                Logger::log(LogLevel::ERROR, "Texture not found for name: " + name);
                return {};
            }
    };
};
