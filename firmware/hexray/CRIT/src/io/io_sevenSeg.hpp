#pragma once
#include "util_errorCodes.hpp"

#include <array>

namespace io::seven_seg
{
inline constexpr size_t DIGITS = 9;

__attribute__((packed)) struct digit
{
    bool c : 1;
    bool dot : 1;
    bool b : 1;
    bool a : 1;
    bool f : 1;
    bool g : 1;
    bool e : 1;
    bool d : 1;
    constexpr explicit digit(
        const bool _a,
        const bool _b,
        const bool _c,
        const bool _d,
        const bool _e,
        const bool _f,
        const bool _g,
        const bool _dot)
      : c(_c), dot(_dot), b(_b), a(_a), f(_f), g(_g), e(_e), d(_d)
    {
    }
};
static_assert(sizeof(digit) == 1);

/**
 * Write the given value to the seven segment display
 * @note that the data will be touched, so please consider the buffer invalidated after use
 * @return success of operation
 */
std::expected<void, ErrorCode> write(std::array<digit, DIGITS> &data);
/**
 * Set the brightness of the seven segment display.
 * @param brightness brightness value from 0.0 (off) to 1.0 (max brightness)
 * @return success of operation
 */
std::expected<void, ErrorCode> setBrightness(float brightness);
} // namespace io::seven_seg

inline constexpr io::seven_seg::digit zero{ true, true, true, true, true, true, false, false };
inline constexpr io::seven_seg::digit one{ false, true, true, false, false, false, false, false };
inline constexpr io::seven_seg::digit two{ true, true, false, true, true, false, true, false };
inline constexpr io::seven_seg::digit three{ true, true, true, true, false, false, true, false };
inline constexpr io::seven_seg::digit four{ false, true, true, false, false, true, true, false };
inline constexpr io::seven_seg::digit five{ true, false, true, true, false, true, true, false };
inline constexpr io::seven_seg::digit six{ true, false, true, true, true, true, true, false };
inline constexpr io::seven_seg::digit seven{ true, true, true, false, false, false, false, false };
inline constexpr io::seven_seg::digit eight{ true, true, true, true, true, true, true, false };
inline constexpr io::seven_seg::digit nine{ true, true, true, false, false, true, true, false };
inline constexpr io::seven_seg::digit a{ true, true, true, false, true, true, true, false };
inline constexpr io::seven_seg::digit b{ false, false, true, true, true, true, true, false };
inline constexpr io::seven_seg::digit c{ true, false, false, true, true, true, false, false };
inline constexpr io::seven_seg::digit d{ false, true, true, true, true, false, true, false };
inline constexpr io::seven_seg::digit e{ true, false, false, true, true, true, true, false };
inline constexpr io::seven_seg::digit f{ true, false, false, false, true, true, true, false };
inline constexpr io::seven_seg::digit g{ true, true, true, true, false, true, true, false };
inline constexpr io::seven_seg::digit h{ false, false, true, false, true, true, true, false };
inline constexpr io::seven_seg::digit i{ false, false, false, false, true, false, false, false };
inline constexpr io::seven_seg::digit j{ false, false, true, true, false, false, false, false };
inline constexpr io::seven_seg::digit l{ false, false, false, true, true, true, false, false };
inline constexpr io::seven_seg::digit n{ false, false, true, false, true, false, true, false };
inline constexpr io::seven_seg::digit o{ false, false, true, true, true, false, true, false };
inline constexpr io::seven_seg::digit p{ true, true, false, false, true, true, true, false };
inline constexpr io::seven_seg::digit q{ true, true, true, false, false, true, true, false };
inline constexpr io::seven_seg::digit r{ false, false, false, false, true, false, true, false };
inline constexpr io::seven_seg::digit s{ true, false, true, true, false, true, true, false };
inline constexpr io::seven_seg::digit t{ false, false, false, true, true, true, true, false };
inline constexpr io::seven_seg::digit u{ false, false, true, true, true, false, false, false };
inline constexpr io::seven_seg::digit y{ false, true, true, true, false, true, true, false };
