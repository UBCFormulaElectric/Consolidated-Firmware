#include "app_charger.h"
#include "hw_pwms.h"
#include "io_charger.h"

float app_charger_getAvaliableCurrent()
{
    hw_pwmInput_tick(&evse_pwm_input);
    if(evse_pwm_input.duty_cycle >= 0.1 && evse_pwm_input.duty_cycle <= 0.85)
    {
        return evse_pwm_input.duty_cycle * 100 * 0.6;
    } 
    else if (evse_pwm_input.duty_cycle > 0.85 && evse_pwm_input.duty_cycle <= 0.97)
    {
        return evse_pwm_input.duty_cycle * 100 * 2.5 - 64;
    }
    return -1;
}