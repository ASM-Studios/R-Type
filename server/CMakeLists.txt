cmake_minimum_required(VERSION 3.27.4)
project(r-type_server)

include_directories(include)

set(SOURCE_FILE src/main.cpp)

add_executable(r-type_server ${SOURCE_FILE})
