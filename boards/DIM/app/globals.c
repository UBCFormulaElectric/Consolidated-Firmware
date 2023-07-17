#include "globals.h"
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

static Globals globals_data;
Globals* const globals = &globals_data;

void globals_init(
    Led *imd_led,
    Led *bspd_led,
    Led *shdn_led,
    Led *drive_led,
    Switch *start_switch,
    Switch *aux_switch,
    RgbLed *bms_status_led,
    RgbLed *dcm_status_led,
    RgbLed *dim_status_led,
    RgbLed *fsm_status_led,
    RgbLed *pdm_status_led)
{
    globals->imd_led = imd_led;
    globals->bspd_led = bspd_led;
    globals->shdn_led = shdn_led;
    globals->drive_led = drive_led;

    globals->start_switch = start_switch;
    globals->aux_switch = aux_switch;
    
    globals->bms_status_led = bms_status_led;
    globals->dcm_status_led = dcm_status_led;
    globals->dim_status_led = dim_status_led;
    globals->fsm_status_led = fsm_status_led;
    globals->pdm_status_led = pdm_status_led;
}