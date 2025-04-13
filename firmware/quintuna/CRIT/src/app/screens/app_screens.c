#include "io_rotary.h"
#include "io_shift_register.h"

#include "app_screens.h"
#include "app_canTx.h"

/************************* Global Variables ***************************/
static uint8_t current_screen = 0;
static bool    init_complete  = false;
Screen        *screens[DRIVE_MODE_COUNT];

/*********************** Static Function Declarations ***************************/
static void app_screens_next(void);

/*********************** Function Definitions ***************************/
void app_screens_init(void)
{
    io_rotary_init();
    io_shift_register_seven_seg_init();

    screens[0] = get_main_drive();
    screens[1] = get_indoor();

    io_rotary_setPushCallback(app_screens_next);
    app_screens_update();
    io_shift_register_updateSevenSegRegisters(screens[current_screen]->display_data);
}

void app_screens_update(void)
{
    screens[current_screen]->update();
}

static void app_screens_next(void)
{
    app_screens_update();
    io_shift_register_updateSevenSegRegisters(screens[current_screen]->display_data);
    io_rotary_setClockwiseCallback(screens[current_screen]->cw_callback);
    io_rotary_setCounterClockwiseCallback(screens[current_screen]->ccw_callback);

    current_screen = (uint8_t)((current_screen + 1) % DRIVE_MODE_COUNT);
}
