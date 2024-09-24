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
    _event = sf::Event();

    _addText("fps", "FPS: " + std::to_string(static_cast<int>(frameRateLimit)), sf::Vector2f(_window->getSize().x - 100, 10));
}

void GUI::WindowManager::run() {
    while (_window->isOpen() && _gameState != gameState::QUITING) {
        _window->clear();
        _eventsHandler();
        _displayBackground();
        _displayMenu();
        _fpsCounter();

        _window->display();
    }
}

void GUI::WindowManager::_eventsHandler() {
    while (_window->pollEvent(_event)) {
        if (_event.type == sf::Event::Closed) {
            _window->close();
        }

        if (_event.type == sf::Event::KeyPressed && _event.key.code == sf::Keyboard::Q) {
            setGameState(gameState::QUITING);
        }
        if (_event.type == sf::Event::KeyPressed && _event.key.code == sf::Keyboard::Escape) {
            if (_gameState == gameState::MENUS) continue;
            _menuState = _menuState == menuState::NO_MENU ? menuState::PAUSE_MENU : menuState::NO_MENU;
        }

        for (auto& [_, button] : _buttons) {
            button.update(*_window, _event);
        }
    }
}

void GUI::WindowManager::_displayBackground() const {
    if (const auto background = _spriteManager.getSprite(_currentBackground, 0)) {
        _window->draw(*background);
    }
    if (_menuState == menuState::PAUSE_MENU) {
        if (const auto popup = _spriteManager.getSprite("backgrounds/gray_mask", 0)) {
            _window->draw(*popup);
        }
    }
}

/* Text */

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

/* Button */

void GUI::WindowManager::_addButton(const std::string &id, const Button<> &button) {
    if (_buttons.contains(id)) {
        throw GuiException("Button with id " + id + " already exists");
    }
    _buttons.emplace(id, button);
}

GUI::Button<> GUI::WindowManager::_getButton(const std::string &id) const {
    if (const auto it = _buttons.find(id); it != _buttons.end()) {
        return it->second;
    }
    throw GuiException("Button with id " + id + " not found");
}

void GUI::WindowManager::_deleteButton(const std::string &id) {
    if (const auto it = _buttons.find(id); it != _buttons.end()) {
        _buttons.erase(it);
    }
}

/* Menu */

void GUI::WindowManager::_displayMenu() {
    switch (_menuState) {
        case NO_MENU:
            return;
        case MAIN_MENU:
            _displayMainMenu();
            break;
        case SETTINGS_MENU:
            _displaySettingsMenu();
            break;
        case PAUSE_MENU:
            _displayPauseMenu();
            break;
    }
}

void GUI::WindowManager::_mainMenuInit() {
    constexpr float buttonSpacing = 150.0f;
    const float startY = _window->getSize().y / 2 - buttonSpacing;

    auto playButtonSprites = _spriteManager.getSprites("buttons/play");
    Button<> playButton(playButtonSprites, [this]() {
        this->setGameState(gameState::GAMES);
        this->setMenuState(menuState::NO_MENU);
    }, {_window->getSize().x / 2, startY});
    _addButton("play", playButton);

    auto settingsButtonSprites = _spriteManager.getSprites("buttons/settings");
    Button<> settingsButton(settingsButtonSprites, [this]() {
        this->setGameState(gameState::MENUS);
        this->setMenuState(menuState::SETTINGS_MENU);
    }, {_window->getSize().x / 2, startY + buttonSpacing});
    _addButton("settings", settingsButton);

    auto quitButtonSprites = _spriteManager.getSprites("buttons/quit");
    Button<> quitButton(quitButtonSprites, [this]() {
        this->setGameState(gameState::QUITING);
        _window->close();
    }, {_window->getSize().x / 2, startY + 2 * buttonSpacing});
    _addButton("quit", quitButton);
}

void GUI::WindowManager::_displayMainMenu() {
    if (_buttons.empty()) {
        _mainMenuInit();
    }

    try {
        _getButton("play").draw(*_window);
        _getButton("settings").draw(*_window);
        _getButton("quit").draw(*_window);
    } catch (const GuiException& e) {
        Logger::log(LogLevel::ERROR, e.what());
    }
}

void GUI::WindowManager::_settingsMenuInit() {

}

void GUI::WindowManager::_displaySettingsMenu() {

}

void GUI::WindowManager::_pauseMenuInit() {

}

void GUI::WindowManager::_displayPauseMenu() {

}
