#pragma once

#include <functional>
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>

namespace GUI {
    template<typename... Args>
    class Button {
        std::map<std::size_t, std::shared_ptr<sf::Sprite>> _buttonSprites;
            std::function<void(Args...)> _onClick;
            std::pair<std::size_t, std::size_t> _position;
            std::size_t _currentState = 0;
            bool _isStatic;

            bool isMouseOver(const sf::Vector2i& mousePos) const {
                if (_buttonSprites.contains(0)) {
                    const sf::FloatRect bounds = _buttonSprites.at(0)->getGlobalBounds();
                    return bounds.contains(static_cast<sf::Vector2f>(mousePos));
                }
                return false;
            }

        public:
            Button(const std::map<std::size_t, std::shared_ptr<sf::Sprite>>& buttonSprites, std::function<void(Args...)> onClick, std::pair<std::size_t, std::size_t> position, const bool isStatic = false)
                : _buttonSprites(buttonSprites), _onClick(std::move(onClick)), _position(std::move(position)), _isStatic(isStatic) {
                for (const auto& [_, sprite] : _buttonSprites) {
                    sprite->setPosition(static_cast<float>(_position.first), static_cast<float>(_position.second));
                    sprite->setOrigin(sprite->getLocalBounds().width / 2, sprite->getLocalBounds().height / 2);
                }
            }
            ~Button() = default;

            void click(Args... args) {
                if (_onClick) {
                    _onClick(std::forward<Args>(args)...);
                }
            }

            void setOnClick(std::function<void(Args...)> onClick) {
                _onClick = std::move(onClick);
            }

            void update(const sf::RenderWindow& window, const sf::Event& event) {
                if (_isStatic) { return; }
                std::size_t state = 0;

                if (const sf::Vector2i mousePos = sf::Mouse::getPosition(window); isMouseOver(mousePos)) {
                    state = 1;

                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                        state = 2;
                    }
                    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                        state = 1;
                        click();
                    }
                }

                _currentState = state;
            }

            void draw(sf::RenderWindow& window) {
                if (_isStatic) { _currentState = 0; }
                if (_buttonSprites.contains(_currentState)) {
                    window.draw(*_buttonSprites.at(_currentState));
                }
            }
    };
}
