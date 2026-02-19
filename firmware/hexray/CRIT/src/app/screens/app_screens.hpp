#pragma once
#include "io_rotary.hpp"

namespace app::screens
{
struct Screen
{
    RotaryCallback ccw_callback; // Called on counter clockwise rotary rotation.
    RotaryCallback cw_callback;  // Called on clockwise rotary rotation.
    RotaryCallback update;       // Called every 100Hz cycle to update/broadcast data.
};
void init();
void tick();
} // namespace app::screens