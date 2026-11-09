#pragma once

#include <atomic>
#include <format>
#include <optional>
#include <source_location>
#include <string_view>
#include <utility>

#ifndef SFX_LOG_LEVEL
#error "SFX_LOG_LEVEL must be defined (Off, Error, Warning, Info, Debug or Trace)"
#endif

// Logger with a compile-time upper bound (SFX_LOG_LEVEL) and a runtime level adjustable without recompiling.
// TODO: write to a file (currently std::cout and std::cerr only).
namespace sfx::log {

    enum class Level
    {
        Off,
        Error,
        Warning,
        Info,
        Debug,
        Trace
    };

    inline constexpr Level compiledLevel = Level::SFX_LOG_LEVEL;

    namespace detail {
        inline std::atomic<Level> runtimeLevel{ Level::Info };
        void writeLine(Level level, std::string_view message, const std::source_location& location);
    } // namespace detail

    constexpr bool isCompiled(Level level) noexcept
    {
        return level != Level::Off && level <= compiledLevel;
    }

    inline bool isEnabled(Level level) noexcept
    {
        return level <= detail::runtimeLevel.load(std::memory_order_relaxed);
    }

    inline void setLevel(Level level) noexcept
    {
        detail::runtimeLevel.store(level, std::memory_order_relaxed);
    }

    constexpr std::string_view toString(Level level) noexcept
    {
        switch (level)
        {
            case Level::Off:     return "off";
            case Level::Error:   return "error";
            case Level::Warning: return "warning";
            case Level::Info:    return "info";
            case Level::Debug:   return "debug";
            case Level::Trace:   return "trace";
        }
        return "";
    }

    std::optional<Level> parseLevel(std::string_view name) noexcept;

    template <typename... Args>
    void write(Level level, const std::source_location& location, std::format_string<Args...> format, Args&&... args)
    {
        detail::writeLine(level, std::format(format, std::forward<Args>(args)...), location);
    }

} // namespace sfx::log

#define SFX_LOG(level, ...)                                                             \
    do                                                                                  \
    {                                                                                   \
        if constexpr (::sfx::log::isCompiled(level))                                    \
        {                                                                               \
            if (::sfx::log::isEnabled(level))                                           \
            {                                                                           \
                ::sfx::log::write(level, std::source_location::current(), __VA_ARGS__); \
            }                                                                           \
        }                                                                               \
    } while (false)

#define SFX_LOG_ERROR(...)   SFX_LOG(::sfx::log::Level::Error, __VA_ARGS__)
#define SFX_LOG_WARNING(...) SFX_LOG(::sfx::log::Level::Warning, __VA_ARGS__)
#define SFX_LOG_INFO(...)    SFX_LOG(::sfx::log::Level::Info, __VA_ARGS__)
#define SFX_LOG_DEBUG(...)   SFX_LOG(::sfx::log::Level::Debug, __VA_ARGS__)
#define SFX_LOG_TRACE(...)   SFX_LOG(::sfx::log::Level::Trace, __VA_ARGS__)