#include "io_leds.hpp"

#include "hw_pwmOutputs.hpp"
#include "hw_spis.hpp"

#include <array>

namespace io::leds
{
static uint8_t color_bits(const color c)
{
    return static_cast<uint8_t>(c) & 0b111;
}

std::expected<void, ErrorCode> update(const config &c)
{
    const uint8_t dam   = color_bits(c.dam);
    const bool    dam_r = (dam & 0b001) != 0;
    const bool    dam_g = (dam & 0b010) != 0;
    const bool    dam_b = (dam & 0b100) != 0;

    std::array<uint8_t, 4> led_data{};
    led_data[0] = static_cast<uint8_t>(dam_g << 7) | static_cast<uint8_t>(color_bits(c.vc) << 4) |
                  static_cast<uint8_t>(dam_r << 3) | color_bits(c.shdn);
    led_data[1] = static_cast<uint8_t>(dam_b << 7) | static_cast<uint8_t>(color_bits(c.fsm) << 4) |
                  static_cast<uint8_t>(c.imd << 3) | color_bits(c.crit);
    led_data[2] = static_cast<uint8_t>(color_bits(c.rsm) << 5) | static_cast<uint8_t>(c.imd << 4) |
                  static_cast<uint8_t>(c.ams << 3) | color_bits(c.bms);
    led_data[3] = static_cast<uint8_t>(c.tv << 7) | static_cast<uint8_t>(color_bits(c.launch_control) << 4) |
                  static_cast<uint8_t>(color_bits(c.push_drive) << 1) | c.bspd;
    return leds_device.transmit(led_data);
}

std::expected<void, ErrorCode> setBrightness(const float brightness)
{
    RETURN_IF_ERR_SILENT(led_dimming.start());
    RETURN_IF_ERR_SILENT(led_dimming.setDutyCycle(brightness));
    return {};
}
} // namespace io::leds