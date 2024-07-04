#pragma once
#include "hw_gpio.h"

namespace io
{
class binary_led
{
    const hw::gpio &pin;

  public:
    explicit binary_led(const hw::gpio &pin_in) : pin(pin_in){};

    void set(bool state) const;
};

class led
{
    const hw::gpio &r_pin;
    const hw::gpio &g_pin;
    const hw::gpio &b_pin;

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
    explicit led(const hw::gpio &r_pin_in, const hw::gpio &g_pin_in, const hw::gpio &b_pin_in)
      : r_pin(r_pin_in), g_pin(g_pin_in), b_pin(b_pin_in){};

    void set(RgbColor c) const;
};
} // namespace io