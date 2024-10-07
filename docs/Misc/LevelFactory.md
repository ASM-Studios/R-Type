# Level Factory

The level factory is a static class that help generating the entity in a Registry.<br>
It is used to create the level, the player, the enemies, the items, the obstacles, etc...

## Usage

```cpp

#include "Factories/LevelFactory.hpp"

_registry = new ecs::Registry();

ecs::factory::LevelFactory levelFactory(_registry, {width, height}, "shared/Scenarios/level_1.cfg");
```

### Arguments

- {width, height}: The size of the screen.
- "shared/Scenarios/level_1.cfg": The path to the configuration file.

## Player

**Player MUST be defined as the first entity (id 0)**

## Format

A level is defined in a configuration file with the following format:

```cfg
entities = (
    {
        name = "player";
        components = (
            {
                type = "Position";
                x = 50;
                y = 200;
            },
            {
                type = "Sprite";
                spriteID = 22;  # Ship_1
                stateID = 0;
            },
            {
                type = "AI";
                model = "none";
            }
        );
    },
    {
        name = "enemy";
        components = (
            {
                type = "Position";
                x = 400;
                y = 200;
            },
            {
                type = "Sprite";
                spriteID = 14;  # Sphere
                stateID = 0;
            },
            {
                type = "AI";
                model = "none";
            }
        );
    }
);
```
