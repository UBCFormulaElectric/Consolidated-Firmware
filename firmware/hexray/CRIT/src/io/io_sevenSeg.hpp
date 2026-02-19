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