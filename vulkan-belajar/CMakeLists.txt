cmake_minimum_required(VERSION 3.12...3.28)

get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT is_multi_config AND NOT (CMAKE_BUILD_TYPE OR DEFINED ENV{CMAKE_BUILD_TYPE}))
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Release default")
endif()

project(belajar-vulkan LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

option(iwyu "Run include-what-you-use" OFF)
if(iwyu)
    find_program(IWYU_EXE NAMES include-what-you-use REQUIRED)
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU_EXE})
endif()

set(SOURCES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

file(GENERATE OUTPUT .gitignore CONTENT "*")

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
    /W4
    /permissive-
    /utf-8
    >
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
    >
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
