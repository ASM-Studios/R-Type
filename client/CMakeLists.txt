cmake_minimum_required(VERSION 3.28.1)
project(r-type_client)

set(CMAKE_CXX_STANDARD 23)

add_compile_options(-Wall -Wextra)
add_compile_options(-Wno-unused-parameter)

set(SOURCE_FILE src/main.cpp)

add_executable(r-type_client ${SOURCE_FILE})
