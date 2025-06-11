#include "app_charger.h"
#include "hw_pwms.h"
#include "io_charger.h"

float app_charger_getAvaliableCurrent()
{
    hw_pwmInput_tick(&evse_pwm_input);
    if (evse_pwm_input.duty_cycle >= 0.1f && evse_pwm_input.duty_cycle <= 0.85f)
    {
        return evse_pwm_input.duty_cycle * 100.0f * 0.6f;
    }
    else if (evse_pwm_input.duty_cycle > 0.85f && evse_pwm_input.duty_cycle <= 0.97f)
    {
        return evse_pwm_input.duty_cycle * 100.0f * 2.5f - 64.0f;
    }
    return -1;
}