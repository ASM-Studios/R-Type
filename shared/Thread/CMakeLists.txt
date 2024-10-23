cmake_minimum_required(VERSION 3.27.4)
project(r-type_thread)

include_directories(include)

set(SOURCE_FILE src/ThreadPool.cpp)

add_library(Thread ${SOURCE_FILE})

target_link_libraries(Thread)
