cmake_minimum_required(VERSION 3.28)

project(ascii-renderer 
    LANGUAGES CXX 
    VERSION 0.0.1
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# # --- FETCH FTXUI ---
# include(FetchContent)
# set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
# set(FETCHCONTENT_QUIET OFF)
# FetchContent_Declare(ftxui
#     GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
#     GIT_TAG v6.1.9
#     GIT_PROGRESS TRUE
#     GIT_SHALLOW TRUE
#     EXCLUDE_FROM_ALL TRUE
# )
# FetchContent_MakeAvailable(ftxui)

# --- PROJECT ---
add_executable(${PROJECT_NAME}
    main.cpp
    include/GenMath.cpp
    include/Matrix22.cpp
    include/Matrix33.cpp
    include/Matrix44.cpp
    include/Quaternion.cpp
    include/Vector2.cpp
    include/Vector3.cpp
    include/Vector4.cpp
    include/Rasterizer.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(${PROJECT_NAME} PRIVATE /W4 /sdl /EHsc /analyze:quiet)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/pdcurses.lib
#     ftxui::component
#     ftxui::dom
#     ftxui::screen
)
