#pragma once
#include "util_errorCodes.hpp"

#include <array>
#include <cstdint>

namespace io::seven_seg
{
inline constexpr size_t DIGITS = 9;

__attribute__((packed)) struct digit
{
    bool c : 1   = false;
    bool dot : 1 = false;
    bool b : 1   = false;
    bool a : 1   = false;
    bool f : 1   = false;
    bool g : 1   = false;
    bool e : 1   = false;
    bool d : 1   = false;
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

    constexpr digit() = default;
};
static_assert(sizeof(digit) == 1);

/**
 * Write the given value to the seven segment display
 * @note that the data will be touched, so please consider the buffer invalidated after use
 * @return success of operation
 */
[[nodiscard]] std::expected<void, ErrorCode> write(std::array<digit, DIGITS> &data);
/**
 * Set the brightness of the seven segment display.
 * @param brightness brightness value from 0.0 (off) to 1.0 (max brightness)
 * @return success of operation
 */
[[nodiscard]] std::expected<void, ErrorCode> setBrightness(float brightness);

inline constexpr digit zero{ true, true, true, true, true, true, false, false };
inline constexpr digit one{ false, true, true, false, false, false, false, false };
inline constexpr digit two{ true, true, false, true, true, false, true, false };
inline constexpr digit three{ true, true, true, true, false, false, true, false };
inline constexpr digit four{ false, true, true, false, false, true, true, false };
inline constexpr digit five{ true, false, true, true, false, true, true, false };
inline constexpr digit six{ true, false, true, true, true, true, true, false };
inline constexpr digit seven{ true, true, true, false, false, false, false, false };
inline constexpr digit eight{ true, true, true, true, true, true, true, false };
inline constexpr digit nine{ true, true, true, false, false, true, true, false };
inline constexpr digit a{ true, true, true, false, true, true, true, false };
inline constexpr digit b{ false, false, true, true, true, true, true, false };
inline constexpr digit c{ true, false, false, true, true, true, false, false };
inline constexpr digit d{ false, true, true, true, true, false, true, false };
inline constexpr digit e{ true, false, false, true, true, true, true, false };
inline constexpr digit f{ true, false, false, false, true, true, true, false };
inline constexpr digit g{ true, true, true, true, false, true, true, false };
inline constexpr digit h{ false, false, true, false, true, true, true, false };
inline constexpr digit i{ false, false, false, false, true, false, false, false };
inline constexpr digit j{ false, false, true, true, false, false, false, false };
inline constexpr digit k{ false, false, false, false, true, true, true, false };
inline constexpr digit l{ false, false, false, true, true, true, false, false };
inline constexpr digit m{ true, false, true, false, true, false, false, false };
inline constexpr digit n{ false, false, true, false, true, false, true, false };
inline constexpr digit o{ false, false, true, true, true, false, true, false };
inline constexpr digit p{ true, true, false, false, true, true, true, false };
inline constexpr digit q{ true, true, true, false, false, true, true, false };
inline constexpr digit r{ false, false, false, false, true, false, true, false };
inline constexpr digit s{ true, false, true, true, false, true, true, false };
inline constexpr digit t{ false, false, false, true, true, true, true, false };
inline constexpr digit u{ false, false, true, true, true, false, false, false };
inline constexpr digit v{ false, true, false, true, false, true, false, false };
inline constexpr digit y{ false, true, true, true, false, true, true, false };
inline constexpr digit dot{ false, false, false, false, false, false, false, true };

inline digit digit_to_segment(const uint8_t digit)
{
    switch (digit)
    {
        default:
        case 0:
            return zero;
        case 1:
            return one;
        case 2:
            return two;
        case 3:
            return three;
        case 4:
            return four;
        case 5:
            return five;
        case 6:
            return six;
        case 7:
            return seven;
        case 8:
            return eight;
        case 9:
            return nine;
    }
}
} // namespace io::seven_seg
