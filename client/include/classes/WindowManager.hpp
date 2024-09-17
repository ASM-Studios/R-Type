#pragma once

#include <memory>
#include <string>
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <utility>

#include "GuiException.hpp"

constexpr auto FONT_FILENAME = "client/assets/fonts/arial.ttf";

constexpr std::size_t POPUP_WIDTH = 200;
constexpr std::size_t POPUP_HEIGHT = 100;

constexpr std::size_t WINDOW_WIDTH = 800;
constexpr std::size_t WINDOW_HEIGHT = 600;

constexpr std::size_t FONT_SIZE = 24;
constexpr std::size_t TEXT_POS = 10;

namespace GUI {
    class WindowManager : public std::enable_shared_from_this<WindowManager>
    {
        private:
            std::unique_ptr<sf::RenderWindow> _window;
            sf::Font _font;

        public:
            WindowManager();
            ~WindowManager();

            void run() const;
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
