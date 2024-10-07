cmake_minimum_required(VERSION 3.27.4)
project(r-type-shared)

include_directories(include)

set(SOURCE_FILE src/Logger.cpp)

add_library(Logger ${SOURCE_FILE})

target_link_libraries(Logger Config)
