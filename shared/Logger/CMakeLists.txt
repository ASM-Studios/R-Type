cmake_minimum_required(VERSION 3.27.4)
project(r-type-shared)

include_directories(include)

find_package(Boost COMPONENTS property_tree CONFIG REQUIRED)
find_package(libconfig CONFIG REQUIRED)

set(SOURCE_FILE src/Logger.cpp)

add_library(Logger ${SOURCE_FILE})

target_link_libraries(Logger
    Boost::property_tree
)
