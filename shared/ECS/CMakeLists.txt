cmake_minimum_required(VERSION 3.27.4)
project(r-type-shared)

include_directories(include)
include_directories(src/Factories)

set(SOURCE_FILE src/Registry.cpp
    src/RegistryManager.cpp
    src/Entity.cpp
    src/Position.cpp
    src/Sprite.cpp
    src/Factories/LevelFactory.cpp
    src/Collision.cpp
    src/Behavior.cpp
)

add_library(ECS ${SOURCE_FILE})

target_link_libraries(ECS
    GameLogic Texture)
