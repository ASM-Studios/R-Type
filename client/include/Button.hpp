#pragma once

#include <functional>
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>

namespace GUI {
    template<typename... Args>
    class Button {
        private:
            std::map<std::size_t, std::shared_ptr<sf::Sprite>> _buttonSprites;
            std::function<void(Args...)> _onClick;
            std::pair<std::size_t, std::size_t> _position;
            std::size_t _currentState = 0;

            bool isMouseOver(const sf::Vector2i& mousePos) const {
                if (const auto sprite = _buttonSprites.at(0)) {
                    const sf::FloatRect bounds = sprite->getGlobalBounds();
                    return bounds.contains(static_cast<sf::Vector2f>(mousePos));
                }
                return false;
            }

        public:
            Button(const std::map<std::size_t, std::shared_ptr<sf::Sprite>>& buttonSprites, std::function<void(Args...)> onClick, std::pair<std::size_t, std::size_t> position)
                : _buttonSprites(buttonSprites), _onClick(std::move(onClick)), _position(std::move(position)) {
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

            void draw(sf::RenderWindow& window) const {
                if (const auto sprite = _buttonSprites.at(_currentState)) {
                    window.draw(*sprite);
                }
            }
    };
}
