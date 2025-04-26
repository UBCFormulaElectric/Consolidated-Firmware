#include "app_screens.h"
#include "app_canTx.h"
#include <app_canRx.h>
#include <stdbool.h>
#include <stdint.h>
#include "io_shift_register.h"

/*********************** Function Definitions ***************************/
static void start_up_update(void)
{
    uint8_t data_buffer[SEVEN_SEG_DATA_LENGTH];

    // Get inputs.
    bool air_minus        = true;
    bool inverters_detect = true;
    bool bms_on           = true;
    bool bms_precharge    = true;
    bool bms_drive        = true;
    bool vc_pcm_on        = true;
    bool pcm_detected     = true;
    bool vc_hv_init       = true;
    bool vc_hv            = true;

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
