cmake_minimum_required(VERSION 3.27.4)
project(r-type-shared)

find_package(Boost COMPONENTS gil CONFIG REQUIRED)

include_directories(include)

set(SOURCE_FILE src/Texture.cpp
    src/TextureLoader.cpp)

add_library(Texture ${SOURCE_FILE})

target_link_libraries(Texture
    Boost::gil
)
