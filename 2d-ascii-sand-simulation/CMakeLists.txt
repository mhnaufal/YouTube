cmake_minimum_required(VERSION 3.28)

project(2dasciisandsimulation C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    main.c
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/pdcurses.lib
)

target_compile_options(${PROJECT_NAME} PRIVATE /W4 /sdl /EHsc /MTd /analyze:quiet)
