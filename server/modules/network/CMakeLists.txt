cmake_minimum_required(VERSION 3.27.4)
project(r-type_server)

include_directories(include)
include_directories(../../include)

find_package(Boost COMPONENTS asio CONFIG REQUIRED)

set(SOURCE_FILE src/QueryHandler.cpp
    src/Worker.cpp
    src/socket/Server.cpp
    src/socket/Query.cpp)

add_library(network ${SOURCE_FILE})

target_link_libraries(network Boost::asio)
