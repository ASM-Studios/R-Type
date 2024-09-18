cmake_minimum_required(VERSION 3.27.4)
project(r-type_server)

include_directories(include)

find_package(Boost COMPONENTS asio CONFIG REQUIRED)

set(SOURCE_FILE src/main.cpp
    src/socket/Query.cpp
    src/socket/Server.cpp
    src/QueryHandler.cpp
    src/Worker.cpp)

add_executable(r-type_server ${SOURCE_FILE})

target_link_libraries(r-type_server Boost::asio)
