#include "WindowManager.hpp"
#include "GameLogicMode.hpp"
#include "QueryHandler.hpp"
#include "socket/ServerManager.hpp"
#include <chrono>
#include <thread>

/**
 * \brief Constructs a WindowManager object.
 *
 * Initializes the WindowManager with the default configuration.
 * Loads the configuration from the specified config file.
 * Initializes the window with the specified width and height.
 * Initializes the music manager with the main theme music.
 * Initializes the player entity.
 *
 * \throws GuiException if there is an error reading the config file.
 */
GUI::WindowManager::WindowManager()
    : _player(ecs::RegistryManager::getInstance().getRegistry().createEntity<>(0)),
    _gameLogic(GameLogicMode::CLIENT)
{
    network::socket::udp::ServerManager::getInstance().init();
    const Config &config = Config::getInstance("client/config.json");
    sf::VideoMode const desktop = sf::VideoMode::getDesktopMode();
    _hostname = config.get("hostname").value_or("127.0.0.1");
    _port = std::stoul(config.get("port").value_or("8080"));
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

    _addText("fps", "FPS: " + std::to_string(static_cast<int>(frameRateLimit)), sf::Vector2f(_window->getSize().x - 150, 10));

    _musicManager.setMusic(MAIN_THEME_MUSIC);

    /* ECS Inits */

    ecs::RegistryManager::getInstance().getRegistry().setComponent<ecs::component::Position>(_player, {50, static_cast<int16_t>(height / 2), width, height});
    ecs::RegistryManager::getInstance().getRegistry().setComponent<ecs::component::Sprite>(_player, {22, 2});
    ecs::RegistryManager::getInstance().getRegistry().setComponent<ecs::component::LastShot>(_player, {});
    ecs::RegistryManager::getInstance().getRegistry().setComponent<ecs::component::Input>(_player, {});
    ecs::RegistryManager::getInstance().getRegistry().setComponent<ecs::component::Behavior>(_player, {&BehaviorFunc::handleInput});
}

/**
 * \brief Sets the game state.
 */
void GUI::WindowManager::readServer() {
    auto& server = network::socket::udp::ServerManager::getInstance().getServer();
    if (server.availableRequest()) {
        auto query = server.recv<RawRequest>();
        network::QueryHandler::getInstance().addQuery(query);
    }
    network::QueryHandler::getInstance().executeQueries();
    network::QueryHandler::getInstance().checkWorkers();
}

void GUI::WindowManager::run() {
    _gameLogic.start();
    while (_window->isOpen() && _gameState != gameState::QUITING) {
        std::this_thread::sleep_for(std::chrono::microseconds(50));
        this->readServer();
        _window->clear();
        _eventsHandler();
        _displayBackground();
        if (_gameState == gameState::MENUS || _menuState == menuState::PAUSE_MENU) {
            _displayMenu();
        }
        if (_gameState == gameState::GAMES) {
            _gameLogic.updateTimed();
            _displayGame();
        }
        _fpsCounter();

        _window->display();
    }
}

/**
 * \brief Sets the game state.
 */
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
        ecs::component::Input newInput = registry.getComponent<ecs::component::Input>(_player);
        auto& entityInput = registry.getComponent<ecs::component::Input>(_player);

        newInput.clearFlag(ecs::component::Input::MoveLeft | ecs::component::Input::MoveRight |
                        ecs::component::Input::MoveUp | ecs::component::Input::MoveDown | ecs::component::Input::ReleaseShoot);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            newInput.setFlag(ecs::component::Input::MoveUp);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            newInput.setFlag(ecs::component::Input::MoveDown);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            newInput.setFlag(ecs::component::Input::MoveLeft);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            newInput.setFlag(ecs::component::Input::MoveRight);
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && newInput.isFlagSet((ecs::component::Input::PressedShoot))) {
            newInput.setFlag(ecs::component::Input::ReleaseShoot);
            newInput.clearFlag(ecs::component::Input::PressedShoot);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            newInput.setFlag(ecs::component::Input::PressedShoot);
        }
        entityInput = newInput;
    }
}

/**
 * \brief Sets the game state.
 */
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

/**
 * \brief Adds a text to the window.
 *  The text is identified by its id.
 *  The text is stored and does not need to be recreated every frame.
 *
 * \param id The id of the text.
 * \param text The text to be displayed.
 * \param position The position of the text.
 */
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

/**
 * \brief Gets the text with the specified id.
 *
 * \param id The id of the text.
 *
 * \return The text with the specified id.
 */
std::shared_ptr<sf::Text> GUI::WindowManager::_getText(const std::string& id) const {
    if (const auto it = _texts.find(id); it != _texts.end()) {
        return it->second;
    }
    return nullptr;
}

/**
 * \brief Deletes the text with the specified id.
 *
 * \param id The id of the text.
 */
void GUI::WindowManager::_deleteText(const std::string& id) {
    _texts.erase(id);
}

/**
 * \brief Updates the FPS counter.
 */
void GUI::WindowManager::_fpsCounter() { // Sponsored by Yohan
    if (!_showFps) return;
    static sf::Clock clock;
    static int frameCount = 0;

    frameCount++;
    if (const sf::Time elapsed = clock.getElapsedTime(); elapsed.asSeconds() >= 1.0f) {
        const float fps = std::roundf(frameCount / elapsed.asSeconds());
        frameCount = 0;
        clock.restart();

        auto fpsText = _getText("fps");
        if (!fpsText) {
            _addText("fps", "FPS: 0", sf::Vector2f(_window->getSize().x - 150, 10));
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

/**
 * \brief Adds a button to the window.
 *  The button is identified by its id.
 *  The button is stored and does not need to be recreated every frame.
 *
 * \param id The id of the button.
 * \param button The button to be displayed.
 */
void GUI::WindowManager::_addButton(const std::string &id, const Button<> &button) {
    if (_buttons.contains(id)) {
        throw GuiException("Button with id " + id + " already exists");
    }
    _buttons.emplace(id, button);
}

/**
 * \brief Gets the button with the specified id.
 *
 * \param id The id of the button.
 *
 * \return The button with the specified id.
 *
 * \throws GuiException if the button is not found.
 */
GUI::Button<> GUI::WindowManager::_getButton(const std::string &id) const {
    if (const auto it = _buttons.find(id); it != _buttons.end()) {
        return it->second;
    }
    throw GuiException("Button with id " + id + " not found");
}

/**
 * \brief Deletes the button with the specified id.
 *
 * \param id The id of the button.
 */
void GUI::WindowManager::_deleteButton(const std::string &id) {
    if (const auto it = _buttons.find(id); it != _buttons.end()) {
        _buttons.erase(it);
    }
}

/* Game */

/**
 * \brief Displays the game.
 */
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

/**
 * \brief Displays the menu based on the current game state and menu state.
 */
void GUI::WindowManager::_displayMenu() {
    if (_gameState != _previousGameState || _menuState != _previousMenuState) {
        _currentButtons.clear();
        _previousGameState = _gameState;
        _previousMenuState = _menuState;

        const std::unordered_map<menuState, std::function<void()>> menuInitMap = {
            {NO_MENU, []() {}},
            {MAIN_MENU, [this]() { _mainMenuInit(); }},
            {SCENARIO_SELECTION_MENU, [this]() { _scenarioSelectionInit(); }},
            {SETTINGS_MENU, [this]() { _settingsMenuInit(); }},
            {PAUSE_MENU, [this]() { _pauseMenuInit(); }}
        };

        if (menuInitMap.contains(_menuState)) {
            menuInitMap.at(_menuState)();
        }
    }

    for (auto& [id, button] : _currentButtons) {
        button.draw(*_window);
    }
}

/**
 * \brief Loads the Main Menu.
 */
void GUI::WindowManager::_mainMenuInit() {
    _currentButtons.clear();
    constexpr float buttonSpacing = 150.0f;
    const float startY = _window->getSize().y / 2 - buttonSpacing;

    const auto playButtonSprites = _spriteManager.getSprites("buttons/play");
    const Button<> playButton(playButtonSprites, [this]() {
        this->setGameState(gameState::MENUS);
        this->setMenuState(menuState::SCENARIO_SELECTION_MENU);
    }, {_window->getSize().x / 2, startY});
    _currentButtons.emplace("main:play", playButton);

    const auto settingsButtonSprites = _spriteManager.getSprites("buttons/settings");
    const Button<> settingsButton(settingsButtonSprites, [this]() {
        this->setGameState(gameState::MENUS);
        this->setMenuState(menuState::SETTINGS_MENU);
    }, {_window->getSize().x / 2, startY + buttonSpacing});
    _currentButtons.emplace("main:settings", settingsButton);

    const auto quitButtonSprites = _spriteManager.getSprites("buttons/quit");
    const Button<> quitButton(quitButtonSprites, [this]() {
        this->setGameState(gameState::QUITING);
    }, {_window->getSize().x / 2, startY + 2 * buttonSpacing});
    _currentButtons.emplace("main:quit", quitButton);
}

/**
 * \brief Loads the Scenario Selection Menu.
 */
void GUI::WindowManager::_scenarioSelectionInit() {
    _currentButtons.clear();
    constexpr float buttonSpacing = 150.0f;
    const float startY = _window->getSize().y / 2 - buttonSpacing * 2;
    const float centerX = _window->getSize().x / 2;

    const std::vector<std::string> planetNames = {"terran", "baren", "ice", "lava", "black_hole"};
    for (std::size_t i = 0; i < planetNames.size(); ++i) {
        const auto buttonSprites = _spriteManager.getSprites("buttons/planets/" + planetNames[i]);
        const Button<> planetButton(buttonSprites, [this, i]() {
            // TODO:  Add level loading here where i is the scenario number
            this->setGameState(gameState::GAMES);
            this->setMenuState(menuState::NO_MENU);
        }, {centerX, startY + i * buttonSpacing});
        _currentButtons.emplace("scenario:" + planetNames[i], planetButton);
    }
}

/**
 * \brief Loads the Settings Menu.
 */
void GUI::WindowManager::_settingsMenuInit() {
    _currentButtons.clear();
    constexpr float buttonSpacing = 150.0f;
    const float centerX = _window->getSize().x / 2;
    const float startY = _window->getSize().y / 2 - buttonSpacing;
    const auto musicButtonSpritesWidth = _spriteManager.getSprites("buttons/settings_btn/music").at(0)->getGlobalBounds().width;

    const auto fpsButtonSprites = _spriteManager.getSprites("buttons/settings_btn/fps");
    const Button<> fpsButton(fpsButtonSprites, [this]() {
        _showFps = !_showFps;
    }, {centerX, startY});
    _currentButtons.emplace("settings:fps", fpsButton);

    const auto plusButtonSprites = _spriteManager.getSprites("buttons/settings_btn/plus");
    const Button<> plusButton(plusButtonSprites, [this]() {
        const float volume = _musicManager.getVolume();
        _musicManager.setVolume(volume + 10.0f);
    }, {centerX + musicButtonSpritesWidth / 2 + 45, startY + buttonSpacing});
    _currentButtons.emplace("settings:plus", plusButton);

    const auto minusButtonSprites = _spriteManager.getSprites("buttons/settings_btn/minus");
    const Button<> minusButton(minusButtonSprites, [this]() {
        const float volume = _musicManager.getVolume();
        _musicManager.setVolume(volume - 10.0f);
    }, {centerX - musicButtonSpritesWidth / 2 - 45, startY + buttonSpacing});
    _currentButtons.emplace("settings:minus", minusButton);

    const auto musicButtonSprites = _spriteManager.getSprites("buttons/settings_btn/music");
    const Button<> musicButton(musicButtonSprites, [this]() {
        _musicManager.toggleMute();
    }, {centerX, startY + buttonSpacing});
    _currentButtons.emplace("settings:musics", musicButton);

    const auto mainMenuButtonSprites = _spriteManager.getSprites("buttons/main_menu");
    const Button<> mainMenuButton(mainMenuButtonSprites, [this]() {
        this->setGameState(gameState::MENUS);
        this->setMenuState(menuState::MAIN_MENU);
    }, {centerX, startY + 2 * buttonSpacing});
    _currentButtons.emplace("settings:main_menu", mainMenuButton);
}

/**
 * \brief Loads the Pause Menu.
 */
void GUI::WindowManager::_pauseMenuInit() {
    _currentButtons.clear();
    constexpr float buttonSpacing = 150.0f;
    const float startY = _window->getSize().y / 2 - buttonSpacing;

    const auto resumeButtonSprites = _spriteManager.getSprites("buttons/resume");
    const Button<> resumeButton(resumeButtonSprites, [this]() {
        this->setMenuState(menuState::NO_MENU);
    }, {_window->getSize().x / 2, startY});
    _currentButtons.emplace("pause:resume", resumeButton);

    const auto mainMenuButtonSprites = _spriteManager.getSprites("buttons/main_menu");
    const Button<> mainMenuButton(mainMenuButtonSprites, [this]() {
        this->setGameState(gameState::MENUS);
        this->setMenuState(menuState::MAIN_MENU);
    }, {_window->getSize().x / 2, startY + buttonSpacing});
    _currentButtons.emplace("pause:main_menu", mainMenuButton);

    const auto quitButtonSprites = _spriteManager.getSprites("buttons/quit");
    const Button<> quitButton(quitButtonSprites, [this]() {
        this->setGameState(gameState::QUITING);
    }, {_window->getSize().x / 2, startY + 2 * buttonSpacing});
    _currentButtons.emplace("pause:quit", quitButton);
}

