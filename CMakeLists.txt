cmake_minimum_required(VERSION 3.27.4)
project(rtype)

include_directories(shared/network/include)
include_directories(shared/ECS/include)
include_directories(shared/ECS/src/Factories)
include_directories(shared/Config/include)
include_directories(shared/Logger/include)
include_directories(shared/Texture/include)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(shared)
