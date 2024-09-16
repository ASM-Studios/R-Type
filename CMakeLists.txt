cmake_minimum_required(VERSION 3.27.4)
project(rtype)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_compile_options(-Wall -Wextra)

add_subdirectory(server)
add_subdirectory(client)
