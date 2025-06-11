#include "app_screens.h"
#include "app_screen_defines.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "io_shift_register.h"
#include <app_canUtils.h>

/*********************** Function Definitions ***************************/
static void start_up_update(void)
{
    uint8_t data_buffer[SEVEN_SEG_DATA_LENGTH];

    // Get inputs.
    bool air_minus        = (app_canRx_BMS_State_get() != BMS_INIT_STATE);
    bool inverters_detect = (app_canRx_VC_State_get() >= VC_INVERTER_ON_STATE);
    bool bms_on           = (app_canRx_VC_State_get() >= VC_BMS_ON_STATE);
    bool bms_precharge =
        (app_canRx_BMS_State_get() == BMS_PRECHARGE_CHARGE_STATE ||
         app_canRx_BMS_State_get() == BMS_PRECHARGE_DRIVE_STATE);
    bool bms_drive    = (app_canRx_BMS_State_get() == BMS_DRIVE_STATE);
    bool vc_pcm_on    = (app_canRx_VC_State_get() >= VC_PCM_ON_STATE);
    bool pcm_detected = (app_canRx_VC_State_get() > VC_PCM_ON_STATE);
    bool vc_hv_init   = (app_canRx_VC_State_get() >= VC_HV_INIT_STATE);
    bool vc_hv        = (app_canRx_VC_State_get() >= VC_HV_ON_STATE);

    // Update data buffer.
    data_buffer[0] = air_minus ? SEG_PATTERN_1 : SEG_PATTERN_0;
    data_buffer[1] = inverters_detect ? SEG_PATTERN_1 : SEG_PATTERN_0;
    data_buffer[2] = bms_on ? SEG_PATTERN_1 : SEG_PATTERN_0;
    data_buffer[3] = bms_precharge ? SEG_PATTERN_1 : SEG_PATTERN_0;
    data_buffer[4] = bms_drive ? SEG_PATTERN_1 : SEG_PATTERN_0;
    data_buffer[5] = vc_pcm_on ? SEG_PATTERN_1 : SEG_PATTERN_0;
    data_buffer[6] = pcm_detected ? SEG_PATTERN_1 : SEG_PATTERN_0;
    data_buffer[7] = vc_hv_init ? SEG_PATTERN_1 : SEG_PATTERN_0;
    data_buffer[8] = vc_hv ? SEG_PATTERN_1 : SEG_PATTERN_0;

    // Update seven seg.
    io_shift_register_updateSevenSegRegisters((uint8_t *)data_buffer);
}

Screen start_up_screen = { .ccw_callback = NULL, .cw_callback = NULL, .update = start_up_update };
