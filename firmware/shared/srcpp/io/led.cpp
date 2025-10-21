#include "io/led.hpp"

void io::binary_led::set(bool state) const
{
    pin.writePin(state);
}

void io::led::set(const io::led::RgbColor colour) const
{
    const bool red_on = colour == RgbColor::RED || colour == RgbColor::YELLOW || colour == RgbColor::MAGENTA ||
                        colour == RgbColor::WHITE;
    r_pin.writePin(red_on);

    const bool green_on = colour == RgbColor::GREEN || colour == RgbColor::YELLOW || colour == RgbColor::CYAN ||
                          colour == RgbColor::WHITE;
    g_pin.writePin(green_on);

    const bool blue_on = colour == RgbColor::BLUE || colour == RgbColor::CYAN || colour == RgbColor::MAGENTA ||
                         colour == RgbColor::WHITE;
    b_pin.writePin(blue_on);
}