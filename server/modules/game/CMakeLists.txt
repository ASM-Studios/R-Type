cmake_minimum_required(VERSION 3.27.4)
project(r-type_server)

include_directories(include)
include_directories(../../include)

find_package(Boost COMPONENTS asio CONFIG REQUIRED)

set(SOURCE_FILE src/EntityInfo.cpp
    src/Player.cpp)

add_library(game ${SOURCE_FILE})

target_link_libraries(game Boost::asio)
