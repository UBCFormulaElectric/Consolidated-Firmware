#pragma once

#include "util_utils.hpp"

#include <source_location>
#include <string_view>
#include <array>

/*
This logging module users SEGGER Real-Time Transfer (RTT) for printf-style debugging.
The SEGGER_RTT_printf function has the following spec:
(taken from SEGGER_RTT_printf.c, for reference)

Notes
*    (1) Conversion specifications have following syntax:
*          %[flags][FieldWidth][.Precision]ConversionSpecifier
*    (2) Supported flags:
*          -: Left justify within the field width
*          +: Always print sign extension for signed conversions
*          0: Pad with 0 instead of spaces. Ignored when using '-'-flag or precision
*        Supported conversion specifiers:
*          c: Print the argument as one char
*          d: Print the argument as a signed integer
*          u: Print the argument as an unsigned integer
*          x: Print the argument as an hexadecimal integer
*          s: Print the string pointed to by the argument
*          p: Print the argument as an 8-digit hexadecimal integer. (Argument shall be a pointer to void.)

*/

inline constexpr std::string_view ANSI_BOLD_GREEN  = "\x1B[1;32m";
inline constexpr std::string_view ANSI_BOLD_YELLOW = "\x1B[1;33m";
inline constexpr std::string_view ANSI_BOLD_RED    = "\x1B[1;31m";
inline constexpr std::string_view ANSI_RESET       = "\x1B[1;0m";

// Do not use these macros directly!
#ifdef TARGET_EMBEDDED
#include "SEGGER_RTT.h"
#define _LOG_PRINTF(format, ...) SEGGER_RTT_printf(0, format, ##__VA_ARGS__)
#else
#include <cstdio>
template <typename... Args> void _LOG_PRINTF(const char *format, Args &&...args)
{
    printf(format, args...);
}
#endif

template <std::string_view const &...Strs> struct join
{
    // Join all strings into a single std::array of chars
    static constexpr auto impl() noexcept
    {
        constexpr std::size_t     len = (Strs.size() + ... + 0);
        std::array<char, len + 1> arr{};
        auto                      append = [i = 0, &arr](auto const &s) mutable
        {
            for (auto c : s)
                arr[i++] = c;
        };
        (append(Strs), ...);
        arr[len] = 0;
        return arr;
    }
    // Give the joined string static storage
    static constexpr auto arr = impl();
    // View as a std::string_view
    static constexpr std::string_view value{ arr.data(), arr.size() - 1 };
};
// Helper to get the value out
template <std::string_view const &...Strs> static constexpr auto join_v = join<Strs...>::value;

struct LogContext
{
    const char          *fmt;
    std::source_location loc;

    // The magic: default argument captures the caller's info
    template <typename T>
    // ReSharper disable once CppNonExplicitConvertingConstructor
    LogContext(T &&s, const std::source_location l = std::source_location::current()) : fmt(std::forward<T>(s)), loc(l)
    {
    }
};

template <typename... Args> void LOG(const std::string_view level, const LogContext &ctx, Args &&...args)
{
    _LOG_PRINTF("[%s] %s:%u: ", level.data(), __BASENAME__(ctx.loc.file_name()), ctx.loc.line());
    _LOG_PRINTF(ctx.fmt, std::forward<Args>(args)...);
    _LOG_PRINTF("\r\n");
}

template <typename... Args> void LOG_INFO(const LogContext ctx, Args &&...args)
{
    static constexpr std::string_view level      = "INFO";
    static constexpr std::string_view full_level = join_v<ANSI_BOLD_GREEN, level, ANSI_RESET>;
    LOG(full_level, ctx, std::forward<Args>(args)...);
}

template <typename... Args> void LOG_WARN(const LogContext ctx, Args &&...args)
{
    static constexpr std::string_view level      = "WARN";
    static constexpr std::string_view full_level = join_v<ANSI_BOLD_YELLOW, level, ANSI_RESET>;
    LOG(full_level, ctx, std::forward<Args>(args)...);
}

template <typename... Args> void LOG_ERROR(const LogContext ctx, Args &&...args)
{
    static constexpr std::string_view level      = "ERROR";
    static constexpr std::string_view full_level = join_v<ANSI_BOLD_RED, level, ANSI_RESET>;
    LOG(full_level, ctx, std::forward<Args>(args)...);
}