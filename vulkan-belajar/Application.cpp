#include "Application.h"

#include "Logger.h"

#include <iostream>

namespace vkb
{
auto Application::initialize() -> bool
{
    /// Initialize SDL
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    p_window = SDL_CreateWindow(
        "Vulkan Belajar", i_width, i_height, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);

    if (!p_window)
    {
        std::cerr << "Failed to create SDL Window " << SDL_GetError() << std::endl;
        return false;
    }

    VKB_LOG_INFO("SDL Window created successfully!");

    /// Initialize Vulkan

    return true;
}

auto Application::run() -> void
{
    b_is_running = true;
    while (b_is_running)
    {
        SDL_Event event{};
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                b_is_running = false;
                break;
            }
            else if (event.type == SDL_EVENT_WINDOW_RESIZED)
            {
                i_width = event.window.data1;
                i_height = event.window.data2;
            }
        }
    }
}

auto Application::shutdown() -> void
{
    b_is_running = false;
    SDL_DestroyWindow(p_window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_Quit();
}
} // namespace vkb
