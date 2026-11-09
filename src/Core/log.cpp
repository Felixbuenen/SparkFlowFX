#include "log.h"

#include <array>
#include <iostream>
#include <string>

namespace sfx::log {

    namespace {

        std::string_view fileName(std::string_view path)
        {
            const auto separator = path.find_last_of("/\\");
            return separator == std::string_view::npos ? path : path.substr(separator + 1);
        }

    } // namespace

    std::optional<Level> parseLevel(std::string_view name) noexcept
    {
        constexpr std::array levels{ Level::Off, Level::Error, Level::Warning, Level::Info, Level::Debug, Level::Trace };
        for (const Level level : levels)
        {
            if (toString(level) == name)
            {
                return level;
            }
        }
        return std::nullopt;
    }

    void detail::writeLine(Level level, std::string_view message, const std::source_location& location)
    {
        const std::string line = std::format("[{}] {}:{} | {}\n",
            toString(level),
            fileName(location.file_name()),
            location.line(),
            message);

        auto& stream = level <= Level::Warning ? std::cerr : std::cout;
        stream << line;
    }

} // namespace sfx::log