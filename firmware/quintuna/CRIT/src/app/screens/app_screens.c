#include "io_rotary.h"
#include "io_shift_register.h"

#include "app_screens.h"
#include "app_canTx.h"
#include "app_canRx.h"

/*********************** Static Function Declarations ***************************/
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
    io_rotary_setPushCallback(NULL);

    app_screens_update();
}

/**
 * @brief Screen specific cw rotary callback.
 */
static void app_screens_rotaryCW(void)
{
    if (main_drive_screen.cw_callback != NULL)
    {
        main_drive_screen.cw_callback();
    }
}

/**
 * @brief Screen specific ccw rotary callback.
 */
static void app_screens_rotaryCCW(void)
{
    if (main_drive_screen.ccw_callback != NULL)
    {
        main_drive_screen.ccw_callback();
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
        main_drive_screen.update();
    }
    else
    {
        start_up_screen.update();
    }
}
