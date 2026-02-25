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

/************************* Screens ***************************/
// extern const Screen main_drive_screen;
// extern const Screen drive_modes_screen;
// extern const Screen start_up_screen;
extern const Screen shdn_screen;
extern const Screen alerts_screen;
extern const Screen brightness_screen;

/**
 * @brief Initalize all screens, and starting init screen.
 */
void init();

/**
 * @brief Called every 100Hz, updates seven seg.
 */
void tick();
} // namespace app::screens