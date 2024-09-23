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
    _spriteManager.updateWindowSize(width, height);
    _spriteManager.init();
}

GUI::WindowManager::~WindowManager() = default;

void GUI::WindowManager::run() const {
    while (_window->isOpen()) {
        sf::Event event{};
        while (_window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                _window->close();
            }
        }

        _window->clear();

        if (const auto background = _spriteManager.getSprite("backgrounds/main_menu", 0)) { _window->draw(*background); }

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
