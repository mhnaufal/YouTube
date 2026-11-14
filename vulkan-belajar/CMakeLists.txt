cmake_minimum_required(VERSION 3.12...3.28)

get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT is_multi_config AND NOT (CMAKE_BUILD_TYPE OR DEFINED ENV{CMAKE_BUILD_TYPE}))
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Release default")
endif()

project(belajar-vulkan
    VERSION 0.1.0
    DESCRIPTION "Belajar Vulkan"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE CACHE BOOL "" FORCE)
set(CMAKE_COLOR_DIAGNOSTICS ON)

option(iwyu "Run include-what-you-use" ON)
if(iwyu)
    find_program(IWYU_EXE NAMES include-what-you-use REQUIRED)
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU_EXE})
endif()

set(SOURCES main.cpp Application.cpp)
set(HEADERS Application.h Definitions.h Logger.h Utils.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

file(GENERATE OUTPUT .gitignore CONTENT "*")

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
    /W4
    /permissive-
    /utf-8
    /std:c++20
    /EHsc
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

#######################
# SDL3
#######################
find_package(SDL3 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL3_INCLUDE_DIRS})

#######################
# Vulkan
#######################
find_package(Vulkan REQUIRED COMPONENTS volk shaderc_combined)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan Vulkan::volk Vulkan::shaderc_combined)
target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS})

#######################
# GLM
#######################
find_package(glm REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_include_directories(${PROJECT_NAME} PRIVATE ${glm_INCLUDE_DIRS})

#######################
# spdlog
#######################
find_package(spdlog REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
target_include_directories(${PROJECT_NAME} PRIVATE ${spdlog_INCLUDE_DIRS})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC wsock32 ws2_32)
endif()
