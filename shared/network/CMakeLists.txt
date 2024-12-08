cmake_minimum_required(VERSION 3.27.4)
project(r-type_server)

include_directories(include)
include_directories(../../include)

find_package(Boost COMPONENTS asio CONFIG REQUIRED)

set(SOURCE_FILE src/QueryHandler.cpp
    src/socket/Client.cpp
    src/socket/Server.cpp
    src/socket/NetworkManager.cpp
    src/socket/NRegistry.cpp
    src/query/RawRequest.cpp
    src/query/Header.cpp)

add_library(network ${SOURCE_FILE})

target_link_libraries(network Boost::asio)

if (WIN32 OR MSVC)
    target_link_libraries(r-type_server ws2_32)
endif()
