cmake_minimum_required(VERSION 3.27.4)
project(r-type_client)

include_directories(include)

find_package(SFML COMPONENTS system window graphics network audio CONFIG REQUIRED)

set(SOURCE_FILE src/main.cpp)

add_executable(r-type_client ${SOURCE_FILE})

target_link_libraries(r-type_client sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
