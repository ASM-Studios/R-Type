cmake_minimum_required(VERSION 3.27.4)
project(r-type_server)

add_subdirectory(modules/network)
add_subdirectory(modules/game)

include_directories(include)

find_package(Boost COMPONENTS asio gil CONFIG REQUIRED)
find_package(libconfig CONFIG REQUIRED)
find_package(PNG REQUIRED)

set(SOURCE_FILE src/main.cpp
    src/Clock.cpp
    src/Core.cpp
)

add_executable(r-type_server ${SOURCE_FILE})

target_link_libraries(
    r-type_server
    ECS Config Logger Texture
    network game
    Boost::asio Boost::gil
    PNG::PNG
)

if (WIN32 OR MSVC)
    target_link_libraries(r-type_server ws2_32)
    target_link_libraries(r-type_server libconfig::libconfig libconfig::libconfig++)
elseif (UNIX)
    target_link_libraries(r-type_server libconfig::config libconfig::config++)
endif()
