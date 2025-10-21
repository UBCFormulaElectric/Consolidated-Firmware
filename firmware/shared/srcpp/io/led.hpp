#pragma once
#include "hw/gpio.hpp"

namespace io
{
class binary_led
{
    const hw::Gpio &pin;

  public:
    explicit binary_led(const hw::Gpio &pin_in) : pin(pin_in){};

    void set(bool state) const;
};

class led
{
    const hw::Gpio &r_pin;
    const hw::Gpio &g_pin;
    const hw::Gpio &b_pin;

  public:
    enum class RgbColor
    {
        RED,
        GREEN,
        BLUE,
        YELLOW,
        CYAN,
        MAGENTA,
        WHITE,
        OFF
    };
    explicit led(const hw::Gpio &r_pin_in, const hw::Gpio &g_pin_in, const hw::Gpio &b_pin_in)
      : r_pin(r_pin_in), g_pin(g_pin_in), b_pin(b_pin_in){};

    void set(RgbColor c) const;
};
} // namespace io