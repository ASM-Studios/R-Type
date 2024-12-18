#pragma once

#include <atomic>
#include <memory>
#include <string>
#include <thread>
#include <cmath>
#include <query/RawRequest.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "GuiException.hpp"
#include "SpriteManager.hpp"
#include "MusicManager.hpp"
#include "Button.hpp"
#include "Registry.hpp"
#include "GameLogic.hpp"
#include "socket/Server.hpp"
#include "BehaviorFunc.hpp"
#include "socket/NetworkManager.hpp"
#include "SystemMetrics.hpp"

constexpr auto FONT_FILENAME = "assets/fonts/FFFFORWA.TTF";
constexpr auto MAIN_THEME_MUSIC = "main_theme";

constexpr std::size_t POPUP_WIDTH = 200;
constexpr std::size_t POPUP_HEIGHT = 100;

constexpr std::size_t FONT_SIZE = 24;
constexpr std::size_t TEXT_POS = 10;

constexpr auto MAIN_MENU_BACKGROUND = "backgrounds/main_menu";

constexpr auto MOVEMENT_SPEED = 5;

namespace GUI {
    enum gameState {
        NONE,
        QUITING,
        MENUS,
        GAMES,
    };
    enum menuState {
        NO_MENU,
        MAIN_MENU,
        SCENARIO_SELECTION_MENU,
        SETTINGS_MENU,
        PAUSE_MENU,
    };
    class SystemMetrics;
    class WindowManager : public std::enable_shared_from_this<WindowManager> {
        private:
            std::unique_ptr<sf::RenderWindow> _window;
            sf::Event _event;
            SpriteManager _spriteManager;
            MusicManager _musicManager;
            sf::Font _font;
            std::string _hostname;
            std::size_t _port;
            std::string _currentBackground = MAIN_MENU_BACKGROUND;
            gameState _previousGameState = gameState::NONE;
            menuState _previousMenuState = menuState::NO_MENU;
            gameState _gameState = gameState::MENUS;
            menuState _menuState = MAIN_MENU;
            bool _showInfo = false;
            std::unordered_map<std::string, std::shared_ptr<sf::Text>> _texts;
            std::unordered_map<std::string, Button<>> _buttons;
            std::unordered_map<std::string, Button<>> _currentButtons;
            std::vector<sf::Keyboard::Key> _pressedKeys;
            float _backgroundOffset;
            float _midLayerOffset;
            mutable float _frontLayerOffset;

            std::unique_ptr<GUI::SystemMetrics> _systemMetrics;

            std::atomic<bool> _isRunning;

            ecs::Entity _player;

            void _exit();

            void _eventsHandler();

            void _deleteText(const std::string& id);

            void _addButton(const std::string& id, const Button<>& button);
            GUI::Button<> _getButton(const std::string& id) const;
            void _deleteButton(const std::string& id);

            void _displayBackground();
            void _displayFrontLayer() const;
            void _infoCounter();

            void _displayGame() const;
            void _displayMenu();
            void _mainMenuInit();
            void _scenarioSelectionInit();
            void _settingsMenuInit();
            void _pauseMenuInit();

        public:
            WindowManager();
            ~WindowManager() = default;

            void run();

            void _addText(const std::string& id, const std::string& text, const sf::Vector2f& position);
            std::shared_ptr<sf::Text> _getText(const std::string& id) const;

            void setGameState(const gameState state) {
                _previousGameState = _gameState;
                _gameState = state;
            }
            gameState getGameState() const { return _gameState; }
            void setMenuState(const menuState state) {
                _previousMenuState = _menuState;
                _menuState = state;
            }
            menuState getMenuState() const { return _menuState; }
    };

    class MessagePopup {
        private:
            std::string _message;
            std::thread _thread;

        public:
            explicit MessagePopup(std::string message) : _message(std::move(message)) {
                _thread = std::thread(&MessagePopup::run, this);
                _thread.detach();
            }

            void run() const {
                sf::RenderWindow window(sf::VideoMode(POPUP_WIDTH, POPUP_HEIGHT), "Message");
                sf::Font font;
                if (!font.loadFromFile(FONT_FILENAME)) {
                    throw GuiException("Cannot load font");
                }

                sf::Text text;
                text.setFont(font);
                text.setString(_message);
                text.setCharacterSize(FONT_SIZE);
                text.setPosition(TEXT_POS, TEXT_POS);
                text.setFillColor(sf::Color::White);

                while (window.isOpen()) {
                    sf::Event event{};
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed) {
                            window.close();
                        }
                        if (event.type == sf::Event::KeyPressed) {
                            if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Q) {
                                window.close();
                            }
                        }
                    }

                    window.clear();
                    window.draw(text);
                    window.display();
                }
            }
    };
};
