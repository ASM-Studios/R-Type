cmake_minimum_required(VERSION 3.27.4)
project(r-type-shared)

include_directories(include)

find_package(libconfig CONFIG REQUIRED)

include_directories(${libconfig_INCLUDE_DIRS})

set(SOURCE_FILE src/Config.cpp)

add_library(Config ${SOURCE_FILE})

if (WIN32 OR MSVC)
    target_link_libraries(Config libconfig::libconfig libconfig::libconfig++)
elseif (UNIX)
    target_link_libraries(Config libconfig::config libconfig::config++)
endif()
