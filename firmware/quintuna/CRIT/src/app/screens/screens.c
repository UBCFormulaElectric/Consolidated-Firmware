#include "io/rotary.h"
#include "io/shift_register.h"

#include "app/screens/screens.h"
#include "app_canTx.h"
#include "app_canRx.h"

#define NUM_OF_DRIVE_SCREENS 2u

/************************* Global Variables ***************************/
static uint8_t current_screen = 0;
static Screen  drive_screens[NUM_OF_DRIVE_SCREENS];

/*********************** Static Function Declarations ***************************/
static void app_screens_next(void);
static void app_screens_rotaryCW(void);
static void app_screens_rotaryCCW(void);

/*********************** Function Definitions ***************************/
/**
 * @brief Initalize all screens, and starting init screen.
 */
void app_screens_init(void)
{
    io_rotary_init();
    io_shift_register_seven_seg_init();
    io_rotary_setClockwiseCallback(app_screens_rotaryCW);
    io_rotary_setCounterClockwiseCallback(app_screens_rotaryCCW);
    io_rotary_setPushCallback(app_screens_next);

    drive_screens[0] = main_drive_screen;
    drive_screens[1] = drive_modes_screen;
    app_screens_update();
}

/**
 * @brief Screen specific cw rotary callback.
 */
static void app_screens_rotaryCW(void)
{
    if (drive_screens[current_screen].cw_callback != NULL)
    {
        drive_screens[current_screen].cw_callback();
    }
}

/**
 * @brief Screen specific ccw rotary callback.
 */
static void app_screens_rotaryCCW(void)
{
    if (drive_screens[current_screen].ccw_callback != NULL)
    {
        drive_screens[current_screen].ccw_callback();
    }
}

/**
 * @brief Called every 100Hz, updates seven seg.
 */
void app_screens_update(void)
{
    VCState vc_state = app_canRx_VC_State_get();

    // Display screen, based on VC state.
    if (vc_state == VC_INIT_STATE)
    {
        init_screen.update();
    }
    else if (vc_state == VC_DRIVE_STATE || vc_state == VC_DRIVE_WARNING_STATE)
    {
        drive_screens[current_screen].update();
    }
    else
    {
        start_up_screen.update();
    }
}

/**
 * @brief Rotary push callback, only needed for drive state.
 */
static void app_screens_next(void)
{
    VCState vc_state = app_canRx_VC_State_get();

    // Only multiple screens in drive state.
    if (vc_state == VC_DRIVE_STATE)
    {
        current_screen = (uint8_t)((current_screen + 1) % NUM_OF_DRIVE_SCREENS);
        app_screens_update();
    }
}
