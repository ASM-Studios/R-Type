# Button

To instantiate a button, you need to provide a `WindowManager` and a `SpriteManager`.<br>
The `WindowManager` is used to create the button's window, and the `SpriteManager` is used to create the button's sprite.

Once done, you can call the _addButton method in the `WindowManager` to create the button.

## Storage

Buttons are stored in the `WindowManager` in a `std::unordered_map<std::string, Button<>>`.<br>
As for the text, the `std::string` is the key, and the `Button<>` is the value.

## onClick

The `Button` class has an `onClick` attribute that let you define a function to be called when the button is clicked.<br>

The awaited function is defined as follows:
```cpp
template<typename... Args>
class Button {
    std::function<void(Args...)> _onClick;
}
```

## Usage

To use a button, once created, use the `_getButton` method of `WindowManager`.<br>
Buttons are updated via their `update` method, and drawn via their `draw` method.

The update is called in `_eventsHandler` and is handled by the `WindowManager`.<br>
The draw is called in `_draw` and is to be called manually in the `WindowManager`.
