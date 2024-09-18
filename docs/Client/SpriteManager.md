# Sprite Manager

The sprite manager is responsible for loading and storing all the sprites used in the game.

## Usage

The manager is instantiated in the [`WindowManager`](WindowManager.md) class and is used to load all the sprites in the game.

To get a sprite, use the following code:

```c++
// in the WindowManager class

_spriteManager.getSprite("name", id);
```

- `name` is the name of the sprite sheet.<br>
- `id` is the id of the sprite in the sprite sheet.

Additionally, the sprite manager can be used to get the sprite sheet itself:

```c++
// in the WindowManager class

_spriteManager.getSprites("name");
```

## Sprite Loading

The sprite manager loads the sprites from the `sprites_path` field in the configuration file. (see [Config](Config.md))

Importing only the sprites sheets in the folder will result in an error.<br>
Alongside the sprite, a configuration file is required.

The only supported sprite format is `.png`.

## Folder Structure

You can create folders inside the sprites folder to organize the sprites.

Beware that the configuration file must explicit this with the following format:

```cfg
folder_name: {
  sprite_name: {
    row = 1
    col = 1
    scale = 1.0
  };
}
```

### Configuration File

The configuration file is a `.cfg` file with the following fields:

By default, the configuration file is located at `client/assets/sprites_config.cfg`.<br>
This path can be changed in the [user configuration file](Config.md).

#### example

```cfg
turret: {
  row = 2
  col = 6
  scale = 5.0
};
enemies: {
    sphere: {
        row = 1
        col = 12
        scale = 3.0
    };
}

```

Failure to provide the configuration file will result in an error.
