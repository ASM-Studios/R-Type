cmake_minimum_required(VERSION 3.27.4)
project(r-type_tests)

include_directories(include)

find_package(Boost COMPONENTS stacktrace_basic CONFIG REQUIRED)
find_path(CRITERION_INCLUDE_DIR criterion/criterion.h PATH_SUFFIXES criterion)
find_library(CRITERION_LIBRARY NAMES criterion libcriterion)

add_compile_options(-DBOOST_STACKTRACE_USE_ADDR2LINE)

set(SOURCE_FILES shared.cpp)

add_executable(r-type_tests ${SOURCE_FILES})

target_link_libraries(
        r-type_tests
        criterion
        Boost::stacktrace_basic
        Clock GameLogic ECS network Config Logger Texture dl
)

add_test(NAME empty_registry COMMAND r-type_tests)
add_test(NAME add_entity COMMAND r-type_tests)
add_test(NAME remove_entity COMMAND r-type_tests)
add_test(NAME is_entity_valid COMMAND r-type_tests)
add_test(NAME add_component COMMAND r-type_tests)
add_test(NAME set_component COMMAND r-type_tests)
add_test(NAME get_no_component COMMAND r-type_tests)
add_test(NAME reset_entity COMMAND r-type_tests)
