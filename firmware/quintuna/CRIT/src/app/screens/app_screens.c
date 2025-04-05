#include "app_screens.h"
#include "io_rotary.h"
#include "io_shift_register.h"

static volatile uint8_t current_screen          = 0;
Screen                 *screens[NUM_OF_SCREENS] = { &main_drive, &idk };

void app_screens_update(void)
{
    io_shift_register_updateSevenSegRegisters(screens[current_screen]->display_data, DATA_LENGTH);
}

void app_screens_handle_rotary_clockwise(void)
{
    if (screens[current_screen]->cw_callback != NULL)
    {
        screens[current_screen]->cw_callback();
    }
}

void app_screens_handle_rotary_ccw(void)
{
    if (screens[current_screen]->ccw_callback != NULL)
    {
        screens[current_screen]->ccw_callback();
    }
}

void app_screens_set_current(uint8_t screen_index)
{
    if (screen_index < NUM_OF_SCREENS)
    {
        current_screen = screen_index;
        app_screens_update();
    }
}

void app_screens_next(void)
{
    current_screen = (uint8_t)((current_screen + 1) % NUM_OF_SCREENS);
    app_screens_update();
}
