cmake_minimum_required(VERSION 3.27.4)
project(r-type-shared)

include_directories(include)

set(SOURCE_FILE src/GameLogic.cpp
        src/EntitySchematic.cpp
        src/Collision.cpp
        src/BehaviorFunc.cpp
)

add_library(GameLogic ${SOURCE_FILE})

target_link_libraries(GameLogic ECS Logger Texture)
