# Button

To instantiate a button, you need to provide a `std::map<std::size_t, std::shared_ptr<sf::Sprite>>` for the button's sprites, a `std::function<void(Args...)>` for the button's click event, and a `std::pair<std::size_t, std::size_t>` for the button's position.

Once done, you can call the `_addButton` method in the `WindowManager` to create the button.

## Storage

Buttons are stored in the `WindowManager` in a `std::unordered_map<std::string, Button<>>`.<br>
The `std::string` is the key, and the `Button<>` is the value.

## onClick

The `Button` class has an `onClick` attribute that lets you define a function to be called when the button is clicked.<br>

The awaited function is defined as follows:
    ```cpp
    template<typename... Args>
    class Button {
        std::function<void(Args...)> _onClick;
    };
    ```

## Usage

To use a button, once created, use the `_getButton` method of `WindowManager`.<br>
Buttons are updated via their `update` method, and drawn via their `draw` method.

The update is called in `_eventsHandler` and is handled by the `WindowManager`.<br>
The draw is called in `_displayMenu` and is handled by the `WindowManager`.

### Example

Here is an example of how to create and use a button:

1. **Define Button Sprites**: Create a map of sprites for different button states (e.g., normal, hover, clicked).

    ```cpp
    std::map<std::size_t, std::shared_ptr<sf::Sprite>> buttonSprites;
    buttonSprites[0] = std::make_shared<sf::Sprite>(normalTexture);
    buttonSprites[1] = std::make_shared<sf::Sprite>(hoverTexture);
    buttonSprites[2] = std::make_shared<sf::Sprite>(clickedTexture);
    ```

2. **Create Button**: Instantiate the button with the sprites, click event, and position.

    ```cpp
    GUI::Button<> myButton(buttonSprites, []() {
        std::cout << "Button clicked!" << std::endl;
    }, {100, 100});
    ```

3. **Add Button to WindowManager**: Use the `_addButton` method to add the button to the `WindowManager`.

    ```cpp
    windowManager._addButton("myButton", myButton);
    ```

4. **Update and Draw Button**: The `WindowManager` will handle updating and drawing the button in its `_eventsHandler` and `_displayMenu` methods.

    ```cpp
    void GUI::WindowManager::_eventsHandler() {
        while (_window->pollEvent(_event)) {
            for (auto& [id, button] : _currentButtons) {
                button.update(*_window, _event);
            }
        }
    }

    void GUI::WindowManager::_displayMenu() {
        for (auto& [id, button] : _currentButtons) {
            button.draw(*_window);
        }
    }
    ```
