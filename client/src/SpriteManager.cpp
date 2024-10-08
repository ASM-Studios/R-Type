#include "SpriteManager.hpp"

#include "Config.hpp"

void GUI::SpriteManager::init() {
    Config& config = Config::getInstance("client/config.json");
    _spritesConfigPath = config.get("sprites_config_path").value_or(DEFAULT_SPRITES_CONFIG);
    const auto spritePath = config.get("sprites_path").value_or("");
    if (spritePath.empty()) {
        throw GuiException("No sprites path found in config file");
    }
    loopDirectory(spritePath, "");
}

void GUI::SpriteManager::loopDirectory(const std::string &spritePath, const std::string &relativePath) {
    boost::filesystem::path const path(spritePath + relativePath);
    if (exists(path) && is_directory(path)) {
        for (const auto& entry : boost::filesystem::directory_iterator(path)) {
            if (is_regular_file(entry) && entry.path().extension() == ".png") {
                std::string name = relativePath + boost::algorithm::erase_all_copy(entry.path().filename().string(), ".png");
                importSprites(spritePath, name);
            } else if (is_directory(entry)) {
                std::string newRelativePath = relativePath + entry.path().filename().string() + "/";
                loopDirectory(spritePath, newRelativePath);
            }
        }
    } else {
        std::cerr << "Directory does not exist or is not a directory: " << spritePath << std::endl;
    }
}

void GUI::SpriteManager::importSprites(const std::string& spritePath, const std::string& name) {
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
        for (const auto& [fst, _] : _textures) {
            if (fst == name) {
                throw GuiException("Sprite sheet '" + name + "' already loaded.");
            }
        }
        int row, col;
        if (float scale; setting.lookupValue("row", row) && setting.lookupValue("col", col) && setting.lookupValue("scale", scale)) {
            bool autoscale = setting.lookupValue("autoscale", autoscale) ? autoscale : false;
            bool center = setting.lookupValue("center", center) ? center : true;
            const auto texture = std::make_shared<sf::Texture>();
            if (!texture->loadFromFile(spritePath + "/" + name + ".png")) {
                throw GuiException("Failed to load texture: " + spritePath + "/" + name + ".png");
            }

            _textures[name] = texture;

            std::size_t const spriteWidth = texture->getSize().x / col;
            std::size_t const spriteHeight = texture->getSize().y / row;

            Logger::log(LogLevel::INFO, "Sprite sheet '" + name + "' loaded with " + std::to_string(row * col) + " sprites");

            std::size_t id = 0;
            for (int r = 0; r < row; ++r) {
                for (int c = 0; c < col; ++c) {
                    const auto sprite = std::make_shared<sf::Sprite>();
                    sprite->setTexture(*texture);
                    sprite->setTextureRect(sf::IntRect(c * spriteWidth, r * spriteHeight, spriteWidth, spriteHeight));
                    if (autoscale) {
                        const float scaleFactorX = _width / sprite->getGlobalBounds().width;
                        const float scaleFactorY = _height / sprite->getGlobalBounds().height;
                        sprite->setScale(scaleFactorX, scaleFactorY);
                    } else { sprite->setScale(scale, scale); }
                    if (center) { sprite->setOrigin(spriteWidth / 2, spriteHeight / 2); }
                    _sprites[name][id++] = sprite;
                }
            }
        } else {
            throw GuiException(name + " missing key or wrong typed value in configuration file.");
        }
    } else {
        Logger::log(LogLevel::WARNING, name + " not found in configuration, skipping.");
    }
}

GUI::SpriteManager::~SpriteManager() = default;
