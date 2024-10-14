cmake_minimum_required(VERSION 3.27.4)
project(r-type_server)

include_directories(include)

find_package(libconfig CONFIG REQUIRED)

set(SOURCE_FILE src/main.cpp
    src/Clock.cpp
    src/ScopeDuration.cpp
    src/Core.cpp
    src/registerClient.cpp
)

add_executable(r-type_server ${SOURCE_FILE})

target_link_libraries(
    r-type_server
    network ECS GameLogic Config Logger Texture
)
