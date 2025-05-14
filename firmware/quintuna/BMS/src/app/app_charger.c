#include "app_charger.h"
#include "hw_pwms.h"
#include "io_charger.h"

float app_charger_getAvaliableCurrent()
{
    return evse_pwm_input.duty_cycle * 100 * 0.6;
}