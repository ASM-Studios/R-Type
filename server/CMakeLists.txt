cmake_minimum_required(VERSION 3.27.4)
project(r-type_server)

include_directories(include)

add_compile_options(-DBOOST_STACKTRACE_USE_ADDR2LINE)

set(SOURCE_FILE src/main.cpp
    src/ScopeDuration.cpp
    src/Core.cpp
    src/udpCallback.cpp
    src/tcpCallback.cpp
    src/registerClient.cpp
)

add_executable(r-type_server ${SOURCE_FILE})

target_link_libraries(
    r-type_server
    Clock GameLogic ECS network Config Logger Texture
)
