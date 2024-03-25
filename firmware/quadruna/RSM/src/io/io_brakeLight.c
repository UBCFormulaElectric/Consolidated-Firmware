#include "io_led.h"
#include "app_globals.h"
#include "app_canRx.h"

void brakeLightEnable()
{
    const bool brake_actuated = app_canRx_FSM_BrakeActuated_get();
    io_led_enable(globals->config->brake_light, brake_actuated);
}