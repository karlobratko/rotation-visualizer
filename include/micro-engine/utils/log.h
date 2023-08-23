//
// Created by kbratko on 7/26/2023.
//

#ifndef MICRO_UTILS_LOG_H
#define MICRO_UTILS_LOG_H

#include <chrono>
#include <ctime>

#include "../core/types.h"

namespace micro::logging {
    enum class Level {
        Info    = 0x20,
        Warning = 0x21,
        Error   = 0x1F,
        Fatal   = 0x23
    };

    constexpr fn to_cstring(Level level) noexcept -> core::cstring {
        switch (level) {
            default:
            case Level::Info:
                return "INFO";
            case Level::Warning:
                return "WARN";
            case Level::Error:
                return "ERROR";
            case Level::Fatal:
                return "FATAL";
        }
    }

    auto log(Level level, core::cstring file, core::i32 line) -> std::basic_ostream<char> & {
        time_t    now = std::time(nullptr);
        struct tm tstruct{};
        char      buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%X", &tstruct);
        std::cout << buf << " "
            << "\x1b[" << static_cast<core::i32>(level) << "m" << to_cstring(level) << "\x1b[0m "
            << "\x1b[90m" << file << ":" << line << ":\x1b[0m ";
        return std::cout;
    }

#define cinfo micro::logging::log(micro::logging::Level::Info, __FILE__, __LINE__)

#define cwarn micro::logging::log(micro::logging::Level::Warning, __FILE__, __LINE__)

#define cerror micro::logging::log(micro::logging::Level::Error, __FILE__, __LINE__)

#define cfatal micro::logging::log(micro::logging::Level::Fatal, __FILE__, __LINE__)
}

#endif //MICRO_UTILS_LOG_H
