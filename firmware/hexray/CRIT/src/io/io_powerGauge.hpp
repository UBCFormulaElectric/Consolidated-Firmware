#pragma once
#include "util_errorCodes.hpp"

#include <array>

namespace io::power_gauge
{
inline constexpr std::size_t LEDS = 8;
struct led_frame
{
    uint8_t _s : 3 = 0b111;
    uint8_t _brightness : 5;
    uint8_t _r;
    uint8_t _g;
    uint8_t _b;
    led_frame(const uint8_t brightness, const uint8_t r, const uint8_t g, const uint8_t b)
      : _brightness(brightness), _r(r), _g(g), _b(b)
    {
    }
};
std::expected<void, ErrorCode> update(std::array<led_frame, LEDS> data);
} // namespace io::power_gauge