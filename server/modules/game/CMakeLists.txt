cmake_minimum_required(VERSION 3.27.4)
project(r-type_server)

include_directories(include)
include_directories(../../include)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/..)

set(SOURCE_FILE src/Registry.cpp)

add_library(game SHARED ${SOURCE_FILE})
