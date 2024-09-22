cmake_minimum_required(VERSION 3.27.4)
project(r-type_server)

add_subdirectory(modules/network)
add_subdirectory(modules/game)

include_directories(include)

find_package(Boost COMPONENTS asio CONFIG REQUIRED)

set(SOURCE_FILE src/main.cpp
    src/Logging.cpp
    src/Clock.cpp
    src/Core.cpp)

add_executable(r-type_server ${SOURCE_FILE})

set(NETWORK_LIB ${CMAKE_CURRENT_BINARY_DIR}/modules/libnetwork.so)
set(GAME_LIB ${CMAKE_CURRENT_BINARY_DIR}/modules/libgame.so)

target_link_libraries(r-type_server Boost::asio
    ${NETWORK_LIB}
    ${GAME_LIB})
