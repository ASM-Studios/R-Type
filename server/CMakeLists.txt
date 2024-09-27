cmake_minimum_required(VERSION 3.27.4)
project(r-type_server)

add_subdirectory(modules/network)
add_subdirectory(modules/game)

include_directories(include)

find_package(Boost COMPONENTS asio CONFIG REQUIRED)

set(SOURCE_FILE src/main.cpp
    src/Clock.cpp
    src/Core.cpp
    src/Config.cpp
    src/Logger.cpp
)

add_executable(r-type_server ${SOURCE_FILE})

target_link_libraries(r-type_server Boost::asio
    network
    game
)

if (WIN32 OR MSVC)
    target_link_libraries(r-type_server ws2_32)
endif()
