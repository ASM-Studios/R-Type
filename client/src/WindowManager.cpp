#include "WindowManager.hpp"

GUI::WindowManager::WindowManager() {
    Config &config = Config::getInstance("client/config.json");
    sf::VideoMode const desktop = sf::VideoMode::getDesktopMode();
    const std::size_t width = std::stoul(config.get("width").value_or(std::to_string(desktop.width)));
    const std::size_t height = std::stoul(config.get("height").value_or(std::to_string(desktop.height)));
    _window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(width, height),
        "R-Type",
        config.get("fullscreen").value_or("false") == "true"
            ? sf::Style::Fullscreen
            : (sf::Style::Titlebar | sf::Style::Close)
        );
    const auto frameRateLimit = std::stoul(config.get("frameRateLimit").value_or("60"));
    _window->setFramerateLimit(frameRateLimit);
    _spriteManager.updateWindowSize(width, height);
    _spriteManager.init();

    _addText("fps", "FPS: " + std::to_string(static_cast<int>(frameRateLimit)), sf::Vector2f(_window->getSize().x - 100, 10));
}

GUI::WindowManager::~WindowManager() = default;

void GUI::WindowManager::run() {
    while (_window->isOpen()) {
        sf::Event event{};
        while (_window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                _window->close();
            }
        }

        _window->clear();
        _displayBackground();
        _fpsCounter();

        float x = 50;
        float y = 50;
        for (const auto& [id, snd] : _spriteManager.getSprites("enemies/enemy_1")) {
            constexpr float spriteWidth = 75;
            if (const float windowWidth = _window->getSize().x; x + spriteWidth > windowWidth) {
                x = 50;
                y += spriteWidth;
            }
            snd->setPosition(x, y);
            _window->draw(*snd);
            x += spriteWidth;
        }

        _window->display();
    }
}

void GUI::WindowManager::_addText(const std::string& id, const std::string& text, const sf::Vector2f& position) {
    const auto sfText = std::make_shared<sf::Text>();
    if (!_font.loadFromFile(FONT_FILENAME)) {
        throw GuiException("Cannot load font");
    }
    sfText->setFont(_font);
    sfText->setString(text);
    sfText->setCharacterSize(FONT_SIZE);
    sfText->setPosition(position);
    sfText->setFillColor(sf::Color::White);
    _texts[id] = sfText;
}

std::shared_ptr<sf::Text> GUI::WindowManager::_getText(const std::string& id) const {
    if (const auto it = _texts.find(id); it != _texts.end()) {
        return it->second;
    }
    return nullptr;
}

void GUI::WindowManager::_deleteText(const std::string& id) {
    _texts.erase(id);
}

void GUI::WindowManager::_displayBackground() const {
    if (const auto background = _spriteManager.getSprite(_currentBackground, 0)) {
        _window->draw(*background);
    }
}

void GUI::WindowManager::_fpsCounter() { // Sponsored by Yohan
    static sf::Clock clock;
    static int frameCount = 0;

    frameCount++;
    if (const sf::Time elapsed = clock.getElapsedTime(); elapsed.asSeconds() >= 1.0f) {
        const float fps = frameCount / elapsed.asSeconds();
        frameCount = 0;
        clock.restart();

        auto fpsText = _getText("fps");
        if (!fpsText) {
            _addText("fps", "FPS: 0", sf::Vector2f(_window->getSize().x - 100, 10));
            fpsText = _getText("fps");
        }
        if (fpsText) {
            fpsText->setString("FPS: " + std::to_string(static_cast<int>(fps)));
        }
    }

    if (const auto fpsText = _getText("fps")) {
        _window->draw(*fpsText);
    }
}
