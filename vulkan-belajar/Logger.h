#pragma once

#include <spdlog/spdlog.h>
#include <string>

namespace vkb
{
enum class SPD_LOG_LEVEL
{
    VERBOSE = 0,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL_ERROR,
};

class Logger
{
public:
    static auto set_log_level(SPD_LOG_LEVEL level) -> void;

    template<typename... Args>
    static void log_verbose(fmt::format_string<Args...> fmt_str, Args&&... args)
    {
        std::string message = fmt::format(fmt_str, std::forward<Args>(args)...);
        spdlog::trace("[VKB] {}", message);
    }

    template<typename... Args>
    static void log_debug(fmt::format_string<Args...> fmt_str, Args&&... args)
    {
        std::string message = fmt::format(fmt_str, std::forward<Args>(args)...);
        spdlog::debug("[VKB] {}", message);
    }

    template<typename... Args>
    static void log_info(fmt::format_string<Args...> fmt_str, Args&&... args)
    {
        std::string message = fmt::format(fmt_str, std::forward<Args>(args)...);
        spdlog::info("[VKB] {}", message);
    }

    template<typename... Args>
    static void log_warn(fmt::format_string<Args...> fmt_str, Args&&... args)
    {
        std::string message = fmt::format(fmt_str, std::forward<Args>(args)...);
        spdlog::warn("[VKB] {}", message);
    }

    template<typename... Args>
    static void log_error(fmt::format_string<Args...> fmt_str, Args&&... args)
    {
        std::string message = fmt::format(fmt_str, std::forward<Args>(args)...);
        spdlog::error("[VKB] {}", message);
    }

    template<typename... Args>
    static void log_fatal_error(fmt::format_string<Args...> fmt_str, Args&&... args)
    {
        std::string message = fmt::format(fmt_str, std::forward<Args>(args)...);
        spdlog::critical("[VKB] {}", message);
    }
};
} // namespace vkb

#define VKB_LOG_VERBOSE(...) vkb::Logger::log_verbose(__VA_ARGS__)
#define VKB_LOG_DEBUG(...) vkb::Logger::log_debug(__VA_ARGS__)
#define VKB_LOG_INFO(...) vkb::Logger::log_info(__VA_ARGS__)
#define VKB_LOG_WARN(...) vkb::Logger::log_warn(__VA_ARGS__)
#define VKB_LOG_ERROR(...) vkb::Logger::log_error(__VA_ARGS__)
#define VKB_LOG_FATAL_ERROR(...) vkb::Logger::log_fatal_error(__VA_ARGS__)
