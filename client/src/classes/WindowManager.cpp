#include "classes/WindowManager.hpp"

GUI::WindowManager::WindowManager() {
    sf::VideoMode const desktop = sf::VideoMode::getDesktopMode();
    _window = std::make_unique<sf::RenderWindow>(sf::VideoMode(desktop.width, desktop.height), "R-Type");
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
        _window->display();
    }
}
