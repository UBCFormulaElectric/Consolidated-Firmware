#include "io_rotary.h"
#include "io_shift_register.h"

#include "app_screens.h"

/************************* Global Variables ***************************/
static volatile uint8_t current_screen          = 0;
Screen                 *screens[NUM_OF_SCREENS] = { &main_drive, &accel };

/*********************** Static Function Declarations ***************************/
static void app_screens_next(void);

/*********************** Function Definitions ***************************/
void app_screens_init(void)
{
    io_rotary_init();
    io_rotary_setPushCallback(app_screens_next);
    io_shift_register_updateSevenSegRegisters(screens[current_screen]->display_data, DATA_LENGTH);
}

void app_screens_update(void)
{
    screens[current_screen]->update();
}

static void app_screens_next(void)
{
    current_screen++;

    if (current_screen == NUM_OF_SCREENS)
    {
        current_screen = 0;
    }

    app_screens_update();
    io_shift_register_updateSevenSegRegisters(screens[current_screen]->display_data, DATA_LENGTH);
    io_rotary_setClockwiseCallback(screens[current_screen]->cw_callback);
    io_rotary_setCounterClockwiseCallback(screens[current_screen]->ccw_callback);
}
