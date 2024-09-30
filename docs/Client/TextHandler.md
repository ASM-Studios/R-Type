# Text Handler

The text handling do not have a specific class, but is instead part of `WindowManager`.

## data description

The text storage is defined as follows:

```cpp
std::map<std::string, std::shared_ptr<sf::Text>> _texts;
```

The first string is the id of the text.<br>
The second is a shared pointer to the text object.

## Usage

To create a text, you can use the `addText` function.

To remove a text, you can use the `removeText` function.

To get a text, you can use the `getText` function.

### Update

The `getText` function return a reference to the text object, so you can update it directly.
