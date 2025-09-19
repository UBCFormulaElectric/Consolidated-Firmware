#include "io_charger.h"

#include "hw_pwms.h"
#include "hw_gpios.h"

ChargerConnectedType io_charger_getConnectionStatus()
{
    if (990 <= hw_pwmInput_getFrequency(&evse_pwm_input) && hw_pwmInput_getFrequency(&evse_pwm_input) <= 1010)
        return CHARGER_CONNECTED_EVSE;
    if (hw_gpio_readPin(&n_evse_i_lim_pin))
        return CHARGER_CONNECTED_WALL;
    return CHARGER_DISCONNECTED;
}

float io_charger_getDutyCycle()
{
    return hw_pwmInput_getDutyCycle(&evse_pwm_input);
}
