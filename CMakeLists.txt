cmake_minimum_required(VERSION 3.27.4)
project(rtype)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_subdirectory(client)
add_subdirectory(server)
