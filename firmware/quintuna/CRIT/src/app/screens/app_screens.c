#include "io_rotary.h"
#include "io_shift_register.h"

#include "app_screens.h"
#include "app_canTx.h"

#define NUM_OF_SCREENS 2u

/************************* Global Variables ***************************/
static uint8_t current_screen = 0;
static bool    init_complete  = false;
Screen        *screens[NUM_OF_SCREENS];

/*********************** Static Function Declarations ***************************/
static void app_screens_next(void);
static void app_screens_rotaryCW(void);
static void app_screens_rotaryCCW(void);

/*********************** Function Definitions ***************************/
void app_screens_init(void)
{
    io_shift_register_seven_seg_init();

    io_rotary_setClockwiseCallback(app_screens_rotaryCW);
    io_rotary_setCounterClockwiseCallback(app_screens_rotaryCCW);
    io_rotary_setPushCallback(app_screens_next);

    screens[0] = get_main_drive();
    screens[1] = get_accel();

    app_screens_update();
}

static void app_screens_rotaryCW(void)
{
    if (screens[current_screen]->cw_callback != NULL)
    {
        screens[current_screen]->cw_callback();
    }
}

static void app_screens_rotaryCCW(void)
{
    if (screens[current_screen]->ccw_callback != NULL)
    {
        screens[current_screen]->ccw_callback();
    }
}

void app_screens_update(void)
{
    screens[current_screen]->update();
}

static void app_screens_next(void)
{
    current_screen = (uint8_t)((current_screen + 1) % NUM_OF_SCREENS);

    app_screens_update();
}
