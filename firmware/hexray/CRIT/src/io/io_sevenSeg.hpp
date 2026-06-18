#pragma once
#include "util_errorCodes.hpp"

#include <span>
#include <cstdint>

namespace io::seven_seg
{
inline constexpr size_t DIGITS = 9;

__attribute__((packed)) struct digit
{
    bool d : 1   = false;
    bool e : 1   = false;
    bool g : 1   = false;
    bool f : 1   = false;
    bool a : 1   = false;
    bool b : 1   = false;
    bool dot : 1 = false;
    bool c : 1   = false;
    constexpr explicit digit(
        const bool _a,
        const bool _b,
        const bool _c,
        const bool _d,
        const bool _e,
        const bool _f,
        const bool _g,
        const bool _dot)
      : d(_d), e(_e), g(_g), f(_f), a(_a), b(_b), dot(_dot), c(_c)
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
[[nodiscard]] result<void> write(std::span<char, DIGITS> data);
/**
 * Set the brightness of the seven segment display.
 * @param brightness brightness value from 0.0 (off) to 1.0 (max brightness)
 * @return success of operation
 */
[[nodiscard]] result<void> setBrightness(float brightness);
} // namespace io::seven_seg
