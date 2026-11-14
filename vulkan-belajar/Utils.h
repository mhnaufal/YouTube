#pragma once

#include "Logger.h"

#include <SDL3/SDL_error.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace vkb
{
#define LENGTH(x) (sizeof(x) / sizeof((x)[0]))

#define VKB_SDL_CHECK_ERROR(ERROR_MSG, RETURN)                                                                         \
    do {                                                                                                               \
        const char* error = SDL_GetError();                                                                            \
        if (error == nullptr || *error != '\0')                                                                        \
        {                                                                                                              \
            VKB_LOG_ERROR("{}: {}\n", ERROR_MSG, error);                                                               \
            SDL_ClearError();                                                                                          \
            return RETURN;                                                                                             \
        }                                                                                                              \
    } while (0);
} // namespace vkb
