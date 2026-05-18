#pragma once
#include "util_errorCodes.hpp"

#include <span>
#include <numeric>

namespace io::power_gauge
{
inline constexpr std::size_t LEDS = 8;
struct led_frame
{
    uint8_t _brightness : 5;
    uint8_t _s : 3 = 0b111;
    uint8_t _b;
    uint8_t _g;
    uint8_t _r;
    led_frame(const uint8_t brightness, const uint8_t r, const uint8_t g, const uint8_t b)
      : _brightness(brightness & 0b11111), _b(b), _g(g), _r(r)
    {
    }

    /**
     *
     * @param brightness 0-1
     * @param r 0-1
     * @param g 0-1
     * @param b 0-1
     */
    led_frame(const double brightness, const uint8_t r, const uint8_t g, const uint8_t b)
      : _brightness(static_cast<uint8_t>(brightness * 0b11111) & 0b11111), _b(b), _g(g), _r(r)
    {
    }
};
static_assert(sizeof(led_frame) == 4);

std::expected<void, ErrorCode> update(std::span<led_frame, LEDS> data);
} // namespace io::power_gauge