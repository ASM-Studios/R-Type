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

            std::unordered_map<int, std::shared_ptr<sf::Texture>> _textures;
            std::unordered_map<int, std::map<std::size_t, std::shared_ptr<sf::Sprite>>> _sprites;

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

            int resolveSpriteID(const std::string& name) const {
                libconfig::Config cfg;
                try {
                    cfg.readFile(_spritesConfigPath);
                } catch ([[maybe_unused]] const libconfig::FileIOException &fioex) {
                    throw GuiException("I/O error while reading file.");
                } catch (const libconfig::ParseException &pex) {
                    throw GuiException("Parse error at " + std::string(pex.getFile()) + ":" + std::to_string(pex.getLine()) + " - " + pex.getError());
                }

                if (cfg.exists(name)) {
                    const libconfig::Setting& setting = cfg.lookup(name);
                    if (int spriteID; setting.lookupValue("id", spriteID)) {
                        return spriteID;
                    } else {
                        throw GuiException("ID not found for sprite: " + name);
                    }
                } else {
                    throw GuiException("Sprite name not found in configuration: " + name);
                }
            }

            std::shared_ptr<sf::Texture> getTexture(const int spriteID) const {
                if (const auto it = _textures.find(spriteID); it != _textures.end()) {
                    return it->second;
                }
                Logger::log(LogLevel::ERR, "Texture not found for spriteID: " + spriteID);
                return {};
            }

            std::shared_ptr<sf::Texture> getTexture(const std::string& name) const {
                try {
                    const int spriteID = resolveSpriteID(name);
                    if (const auto it = _textures.find(spriteID); it != _textures.end()) {
                        return it->second;
                    }
                    Logger::log(LogLevel::ERR, "Texture not found for name: " + name);
                    return {};
                } catch (GuiException& e) {
                    Logger::log(LogLevel::ERR, e.what());
                    return {};
                }
            }

            std::shared_ptr<sf::Sprite> getSprite(const int spriteID, const std::size_t stateID) const {
                if (const auto it = _sprites.find(spriteID); it != _sprites.end()) {
                    if (const auto spriteIt = it->second.find(stateID); spriteIt != it->second.end()) {
                        return spriteIt->second;
                    }
                    Logger::log(LogLevel::ERR, "Sprite ID not found for spriteID: " + spriteID);
                    return {};
                }
                Logger::log(LogLevel::ERR, "Sprite not found for spriteID: " + std::to_string(spriteID) + " and stateID: " + std::to_string(stateID));
                return {};
            }

            std::shared_ptr<sf::Sprite> getSprite(const std::string& name, const std::size_t stateID) const {
                try {
                    const int spriteID = resolveSpriteID(name);
                    if (const auto it = _sprites.find(spriteID); it != _sprites.end()) {
                        if (const auto spriteIt = it->second.find(stateID); spriteIt != it->second.end()) {
                            return spriteIt->second;
                        }
                        Logger::log(LogLevel::ERR, "Sprite ID not found for name: " + name);
                        return {};
                    }
                    Logger::log(LogLevel::ERR, "Sprite not found for name: " + name + " and stateID: " + std::to_string(stateID));
                    return {};
                } catch (GuiException& e) {
                    Logger::log(LogLevel::ERR, e.what());
                    return {};
                }
            }

            std::map<std::size_t, std::shared_ptr<sf::Sprite>> getSprites(const int spriteID) const {
                if (const auto it = _sprites.find(spriteID); it != _sprites.end()) {
                    return it->second;
                }
                Logger::log(LogLevel::ERR, "Sprite not found for spriteID " + spriteID);
                return {};
            }

            std::map<std::size_t, std::shared_ptr<sf::Sprite>> getSprites(const std::string& name) const {
                try {
                    const int spriteID = resolveSpriteID(name);
                    if (const auto it = _sprites.find(spriteID); it != _sprites.end()) {
                        return it->second;
                    }
                    Logger::log(LogLevel::ERR, "Sprite not found for name: " + name);
                    return {};
                } catch (GuiException& e) {
                    Logger::log(LogLevel::ERR, e.what());
                    return {};
                }
            }
    };
};
