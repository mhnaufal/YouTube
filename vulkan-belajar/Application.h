#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>

namespace vkb
{
class Application
{
public:
    auto initialize() -> bool;
    auto run() -> void;
    auto shutdown() -> void;

private:
    bool b_is_running = false;
    SDL_Window* p_window = nullptr;

    int i_width = 1024;
    int i_height = 768;
};
} // namespace vkb