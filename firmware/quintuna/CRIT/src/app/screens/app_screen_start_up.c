#include "app_screens.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "io_shift_register.h"

/*********************** Function Definitions ***************************/
static void start_up_update(void)
{
    uint8_t data_buffer[SEVEN_SEG_DATA_LENGTH];

    // Get inputs.
    bool air_minus        = (app_canRx_BMS_State_get() != BMS_INIT_STATE);
    bool inverters_detect = (app_canRx_VC_State_get() >= VC_INVERTER_ON_STATE);
    bool bms_on           = (app_canRx_VC_State_get() >= VC_BMS_ON_STATE);
    bool bms_precharge    = (app_canRx_BMS_State_get() >= BMS_PRECHARGE_STATE);
    bool bms_drive        = (app_canRx_BMS_State_get() == BMS_DRIVE_STATE);
    bool vc_pcm_on        = (app_canRx_VC_State_get() >= VC_PCM_ON_STATE);
    bool pcm_detected     = (app_canRx_VC_State_get() > VC_PCM_ON_STATE);
    bool vc_hv_init       = (app_canRx_VC_State_get() >= VC_HV_INIT_STATE);
    bool vc_hv            = (app_canRx_VC_State_get() >= VC_HV_STATE);

    // Update data buffer.
    data_buffer[0] = air_minus;
    data_buffer[1] = inverters_detect;
    data_buffer[2] = bms_on;
    data_buffer[3] = bms_precharge;
    data_buffer[4] = bms_drive;
    data_buffer[5] = vc_pcm_on;
    data_buffer[6] = pcm_detected;
    data_buffer[7] = vc_hv_init;
    data_buffer[8] = vc_hv;

    // Update seven seg.
    io_shift_register_updateSevenSegRegisters((uint8_t *)data_buffer);
}

Screen *get_start_up_screen(void)
{
    static Screen start_up = { .ccw_callback = NULL, .cw_callback = NULL, .update = start_up_update };
    return &start_up;
}
