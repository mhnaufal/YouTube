cmake_minimum_required(VERSION 3.28)

project(2d-ascii-shader 
    LANGUAGES CXX 
    VERSION 0.0.1
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- CONAN ---
find_package(ftxui REQUIRED)

# --- PROJECT ---
add_executable(${PROJECT_NAME}
    main.cpp
)

target_compile_options(${PROJECT_NAME} PRIVATE /W4 /sdl /EHsc /analyze:quiet)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ftxui::component
    ftxui::dom
    ftxui::screen
)
