#include "Application.h"

#include "Logger.h"

namespace vkb
{
auto Application::initialize() -> bool
{
    /// Initialize SDL
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        VKB_LOG_ERROR("Failed to initialize SDL: {}", SDL_GetError());
        return false;
    }

    auto p_temp_window = SDL_CreateWindow(
        "Vulkan Belajar", i_width, i_height, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);

    p_window.reset(&p_temp_window);

    if (!p_window)
    {
        VKB_LOG_ERROR("Failed to create SDL Window: {}", SDL_GetError());
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

    SDL_DestroyWindow(*p_window.get());

    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_Quit();

    VKB_LOG_INFO("Application shutdown successfully!");
}
} // namespace vkb
