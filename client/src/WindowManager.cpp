#include "WindowManager.hpp"

GUI::WindowManager::WindowManager() {
    Config& config = Config::getInstance("client/config.json");
    sf::VideoMode const desktop = sf::VideoMode::getDesktopMode();
    _window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(desktop.width, desktop.height),
        "R-Type",
        config.get("fullscreen").value_or("false") == "true" ? sf::Style::Fullscreen : sf::Style::Default
    );
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

        float x = 50;
        float y = 50;
        for (const auto& [id, snd] : _spriteManager.getSprites("enemies/sphere")) {
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
