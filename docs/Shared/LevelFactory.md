# Level Factory

The level factory is a static class that help generating the entity in a Registry.<br>
It is used to create the level, the player, the enemies, the items, the obstacles, etc...

## Usage

```cpp

#include "Factories/LevelFactory.hpp"

_registry = new ecs::Registry();

ecs::factory::LevelFactory::load(_registry, {width, height}, "shared/Scenarios/level_1.cfg");
```

### Arguments

- {width, height}: The size of the screen.
- "shared/Scenarios/level_1.cfg": The path to the configuration file.

## Path

You can use the `getScenarioPath()` function to get the path of the scenario.

```cpp
ecs::factory::LevelFactory::load({width, height}, ecs::factory::getScenarioPath(1));
```

## Player

The player entity **SHOULD NOT** be created in the scenario, it should be created directly by the client and/or the server.

Client side, the player will always have the id 0.

The server will handle player with its own id.

## Minimal ID

In the scenario configuration, **DO NOT** put and id lower than 10.<br>
Entities with an id lower than 10 are reserved for the engine and will be ignored.

## Format

A level is defined in a configuration file with the following format:

```cfg
entities = (
   {
        name = "enemy";
        id = 11;
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
