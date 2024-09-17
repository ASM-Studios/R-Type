#include "SpriteManager.hpp"

GUI::SpriteManager::SpriteManager() {
    Config& config = Config::getInstance(DEFAULT_CONFIG);
    const auto spritePath = config.get("sprites_path").value_or("");
    if (spritePath.empty()) {
        throw GuiException("No sprites path found in config file");
    }

    if (boost::filesystem::path const path(spritePath); boost::filesystem::exists(path) && boost::filesystem::is_directory(path)) {
        for (const auto& entry : boost::filesystem::directory_iterator(path)) {
            importSprites(spritePath, boost::algorithm::erase_all_copy(entry.path().string().substr(spritePath.length()), ".png"));
        }
    } else {
        std::cerr << "Directory does not exist or is not a directory: " << spritePath << std::endl;
    }
};

void GUI::SpriteManager::importSprites(const std::string& spritePath, const std::string& name) {
    libconfig::Config cfg;
    try {
        cfg.readFile(SPRITES_CONFIG);
    } catch (const libconfig::FileIOException &fioex) {
        throw GuiException("I/O error while reading file.");
    } catch (const libconfig::ParseException &pex) {
        throw GuiException("Parse error at " + std::string(pex.getFile()) + ":" + std::to_string(pex.getLine()) + " - " + pex.getError());
    }

    if (cfg.exists(name)) {
        const libconfig::Setting& setting = cfg.lookup(name);
        int row, col;
        if (float scale; setting.lookupValue("row", row) && setting.lookupValue("col", col) && setting.lookupValue("scale", scale)) {
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
                    sprite->setScale(scale, scale);
                    sprite->setOrigin(spriteWidth / 2, spriteHeight / 2);
                    _sprites[name][id++] = sprite;
                }
            }
        } else {
            throw GuiException(name + " settings are incomplete.");
        }
    } else {
        Logger::log(LogLevel::WARNING, name + " not found in configuration, skipping.");
    }
};

GUI::SpriteManager::~SpriteManager() = default;
