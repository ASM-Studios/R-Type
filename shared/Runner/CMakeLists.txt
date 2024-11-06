cmake_minimum_required(VERSION 3.27.4)
project(r-type-shared)

include_directories(include)

find_package(Boost COMPONENTS gil CONFIG REQUIRED)

set(SOURCE_FILE src/ProceduralGenerator.cpp)

add_library(Runner ${SOURCE_FILE})

target_link_libraries(Runner ECS Logger Texture)
