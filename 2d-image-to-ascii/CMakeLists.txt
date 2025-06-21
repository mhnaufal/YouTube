cmake_minimum_required(VERSION 3.28)

project(2dimagetoascii C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    stb_image.h
    main.c
)

target_compile_options(${PROJECT_NAME} PRIVATE /W4 /sdl /EHsc /MTd /analyze:quiet)

# file(COPY resources/ DESTINATION ${CMAKE_BINARY_DIR}/Debug/resources)
