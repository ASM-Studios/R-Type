#include "WindowManager.hpp"

GUI::WindowManager::WindowManager()
: _player(ecs::RegistryManager::getInstance().getRegistry().createEntity<>()) {
    const Config &config = Config::getInstance("client/config.json");
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

    _musicManager.setMusic(MAIN_THEME_MUSIC);

    /* ECS Inits */

    ecs::RegistryManager::getInstance().getRegistry().setComponent<ecs::component::Position>(_player, {50, static_cast<int16_t>(height / 2), width, height});
    ecs::RegistryManager::getInstance().getRegistry().setComponent<ecs::component::Sprite>(_player, {22, 0});

    ecs::factory::LevelFactory::load({width, height}, ecs::factory::getScenarioPath(1));
    // Changing level:
    // ecs::RegistryManager::getInstance().getRegistry().resetAll();
    // ecs::factory::LevelFactory::load({width, height}, "shared/Scenarios/level_2.cfg");
}

void GUI::WindowManager::run() {
    while (_window->isOpen() && _gameState != gameState::QUITING) {
        _window->clear();
        _eventsHandler();
        _displayBackground();
        if (_gameState == gameState::MENUS || _menuState == menuState::PAUSE_MENU) {
            _displayMenu();
        }
        if (_gameState == gameState::GAMES) {
            _displayGame();
        }
        _fpsCounter();

        _window->display();
    }
}

void GUI::WindowManager::_eventsHandler() {
    while (_window->pollEvent(_event)) {
        if (_event.type == sf::Event::Closed) {
            _window->close();
        }

        if (_event.type == sf::Event::KeyPressed) {
            if (_event.key.code == sf::Keyboard::Q) {
                setGameState(gameState::QUITING);
            }
            if (_event.key.code == sf::Keyboard::Escape) {
                if (_gameState == gameState::MENUS) continue;
                _menuState = _menuState == menuState::NO_MENU ? menuState::PAUSE_MENU : menuState::NO_MENU;
            }
        }

        for (auto& [id, button] : _currentButtons) {
            button.update(*_window, _event);
        }
    }

    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    if (_gameState == gameState::GAMES) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            registry.getComponent<ecs::component::Position>(_player).move(ecs::component::Position(0, -MOVEMENT_SPEED));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            registry.getComponent<ecs::component::Position>(_player).move(ecs::component::Position(0, MOVEMENT_SPEED));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            registry.getComponent<ecs::component::Position>(_player).move(ecs::component::Position(-MOVEMENT_SPEED, 0));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            registry.getComponent<ecs::component::Position>(_player).move(ecs::component::Position(MOVEMENT_SPEED, 0));
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
    if (!_showFps) return;
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

/* Game */

void GUI::WindowManager::_displayGame() const {
    for (ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry(); const auto& entity : registry.getEntities()) {
        if (registry.contains<ecs::component::Sprite>(entity) && registry.contains<ecs::component::Position>(entity)) {
            const auto [spriteID, stateID] = registry.getComponent<ecs::component::Sprite>(entity).getSpriteState();
            const auto sprite = _spriteManager.getSprite(spriteID, stateID);
            const auto [x, y] = registry.getComponent<ecs::component::Position>(entity).get();
            sprite->setPosition(x, y);
            _window->draw(*sprite);
        }
    }
}

/* Menu */

void GUI::WindowManager::_displayMenu() {
    if (_gameState != _previousGameState || _menuState != _previousMenuState) {
        _currentButtons.clear();
        _previousGameState = _gameState;
        _previousMenuState = _menuState;
    }

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

    for (auto& [id, button] : _currentButtons) {
        button.draw(*_window);
    }
}

void GUI::WindowManager::_mainMenuInit() {
    constexpr float buttonSpacing = 150.0f;
    const float startY = _window->getSize().y / 2 - buttonSpacing;

    const auto playButtonSprites = _spriteManager.getSprites("buttons/play");
    const Button<> playButton(playButtonSprites, [this]() {
        this->setGameState(gameState::GAMES);
        this->setMenuState(menuState::NO_MENU);
    }, {_window->getSize().x / 2, startY});
    _addButton("main:play", playButton);

    const auto settingsButtonSprites = _spriteManager.getSprites("buttons/settings");
    const Button<> settingsButton(settingsButtonSprites, [this]() {
        this->setGameState(gameState::MENUS);
        this->setMenuState(menuState::SETTINGS_MENU);
    }, {_window->getSize().x / 2, startY + buttonSpacing});
    _addButton("main:settings", settingsButton);

    const auto quitButtonSprites = _spriteManager.getSprites("buttons/quit");
    const Button<> quitButton(quitButtonSprites, [this]() {
        this->setGameState(gameState::QUITING);
    }, {_window->getSize().x / 2, startY + 2 * buttonSpacing});
    _addButton("main:quit", quitButton);
}

void GUI::WindowManager::_displayMainMenu() {
    std::vector<std::string> buttons = {"main:play", "main:settings", "main:quit"};
    for (const auto& button : buttons) {
        if (!_buttons.contains(button)) {
            try {
                for (const auto& b : buttons) {
                    _deleteButton(b);
                }
            } catch (const GuiException& e) {
                Logger::log(LogLevel::ERR, e.what());
            }
            _mainMenuInit();
        }
    }

    try {
        for (const auto& button : buttons) {
            _currentButtons.emplace(button, _getButton(button));
            _currentButtons.at(button).draw(*_window);
        }
    } catch (const GuiException& e) {
        Logger::log(LogLevel::ERR, e.what());
    }
}

void GUI::WindowManager::_settingsMenuInit() {
    constexpr float buttonSpacing = 150.0f;
    const float centerX = _window->getSize().x / 2;
    const float startY = _window->getSize().y / 2 - buttonSpacing;
    const auto musicButtonSpritesWidth = _spriteManager.getSprites("buttons/settings_btn/music").at(0)->getGlobalBounds().width;

    const auto fpsButtonSprites = _spriteManager.getSprites("buttons/settings_btn/fps");
    const Button<> fpsButton(fpsButtonSprites, [this]() {
        _showFps = !_showFps;
    }, {centerX, startY});
    _addButton("settings:fps", fpsButton);

    const auto plusButtonSprites = _spriteManager.getSprites("buttons/settings_btn/plus");
    const Button<> plusButton(plusButtonSprites, [this]() {
        const float volume = _musicManager.getVolume();
        _musicManager.setVolume(volume + 10.0f);
    }, {centerX + musicButtonSpritesWidth / 2 + 45, startY + buttonSpacing});
    _addButton("settings:plus", plusButton);

    const auto minusButtonSprites = _spriteManager.getSprites("buttons/settings_btn/minus");
    const Button<> minusButton(minusButtonSprites, [this]() {
        const float volume = _musicManager.getVolume();
        _musicManager.setVolume(volume - 10.0f);
    }, {centerX - musicButtonSpritesWidth / 2 - 45, startY + buttonSpacing});
    _addButton("settings:minus", minusButton);

    const auto musicButtonSprites = _spriteManager.getSprites("buttons/settings_btn/music");
    const Button<> musicButton(musicButtonSprites, [this]() {
        _musicManager.toggleMute();
    }, {centerX, startY + buttonSpacing});
    _addButton("settings:musics", musicButton);
    const auto mainMenuButtonSprites = _spriteManager.getSprites("buttons/main_menu");
    const Button<> mainMenuButton(mainMenuButtonSprites, [this]() {
            this->setGameState(gameState::MENUS);
            this->setMenuState(menuState::MAIN_MENU);
    }, {centerX, startY + 2 * buttonSpacing});
    _addButton("settings:main_menu", mainMenuButton);
}

void GUI::WindowManager::_displaySettingsMenu() {
    std::vector<std::string> buttons = {
        "settings:fps",
        "settings:plus",
        "settings:minus",
        "settings:musics",
        "settings:main_menu"
    };

    for (const auto& button : buttons) {
        if (!_buttons.contains(button)) {
            try {
                for (const auto& b : buttons) {
                    _deleteButton(b);
                }
            } catch (const GuiException& e) {
                Logger::log(LogLevel::ERR, e.what());
            }
            _settingsMenuInit();
        }
    }

    try {
        for (const auto& button : buttons) {
            _currentButtons.emplace(button, _getButton(button));
            _currentButtons.at(button).draw(*_window);
        }
    } catch (const GuiException& e) {
        Logger::log(LogLevel::ERR, e.what());
    }
}

void GUI::WindowManager::_pauseMenuInit() {
    constexpr float buttonSpacing = 150.0f;
    const float startY = _window->getSize().y / 2 - buttonSpacing;

    const auto resumeButtonSprites = _spriteManager.getSprites("buttons/resume");
    const Button<> resumeButton(resumeButtonSprites, [this]() {
        this->setMenuState(menuState::NO_MENU);
    }, {_window->getSize().x / 2, startY});
    _addButton("pause:resume", resumeButton);

    const auto mainMenuButtonSprites = _spriteManager.getSprites("buttons/main_menu");
    const Button<> mainMenuButton(mainMenuButtonSprites, [this]() {
        this->setGameState(gameState::MENUS);
        this->setMenuState(menuState::MAIN_MENU);
    }, {_window->getSize().x / 2, startY + buttonSpacing});
    _addButton("pause:main_menu", mainMenuButton);

    const auto quitButtonSprites = _spriteManager.getSprites("buttons/quit");
    const Button<> quitButton(quitButtonSprites, [this]() {
        this->setGameState(gameState::QUITING);
    }, {_window->getSize().x / 2, startY + 2 * buttonSpacing});
    _addButton("pause:quit", quitButton);
}

void GUI::WindowManager::_displayPauseMenu() {
    std::vector<std::string> buttons = {"pause:resume", "pause:main_menu", "pause:quit"};
    for (const auto& button : buttons) {
        if (!_buttons.contains(button)) {
            try {
                for (const auto& b : buttons) {
                    _deleteButton(b);
                }
            } catch (const GuiException& e) {
                Logger::log(LogLevel::ERR, e.what());
            }
            _pauseMenuInit();
        }
    }

    try {
        for (const auto& button : buttons) {
            _currentButtons.emplace(button, _getButton(button));
            _currentButtons.at(button).draw(*_window);
        }
    } catch (const GuiException& e) {
        Logger::log(LogLevel::ERR, e.what());
    }
}
