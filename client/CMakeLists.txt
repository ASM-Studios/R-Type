cmake_minimum_required(VERSION 3.27.4)
project(r-type_client)

include_directories(include)

find_package(SFML COMPONENTS system window graphics network audio CONFIG REQUIRED)
find_package(Boost COMPONENTS filesystem property_tree algorithm CONFIG REQUIRED)

set(SOURCE_FILES
        src/main.cpp
        src/WindowManager.cpp
        src/udpCallback.cpp
        src/tcpCallback.cpp
        src/SpriteManager.cpp
        src/MusicManager.cpp
        src/SystemMetrics.cpp
)

add_executable(runner_client ${SOURCE_FILES})
target_compile_definitions(runner_client PRIVATE RUNNER)
target_link_libraries(
        runner_client
        sfml-system sfml-window sfml-graphics sfml-network sfml-audio
        Boost::filesystem Boost::property_tree Boost::algorithm
        Clock GameLogic ECS network Config Logger Texture
)

add_executable(r-type_client ${SOURCE_FILES})
target_compile_definitions(r-type_client PRIVATE RTYPE)
target_link_libraries(
        r-type_client
        sfml-system sfml-window sfml-graphics sfml-network sfml-audio
        Boost::filesystem Boost::property_tree Boost::algorithm
        Clock GameLogic ECS network Config Logger Texture
)
