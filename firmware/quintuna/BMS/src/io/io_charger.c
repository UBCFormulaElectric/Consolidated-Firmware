#include "hw_pwms.h"
#include "hw_gpios.h"
#include "io_charger.h"
#include "cmsis_os.h"
#include "assert.h"

ConnectionStatus io_charger_getConnectionStatus()
{
    hw_pwmInput_tick(&evse_pwm_input);
    if(990 <= evse_pwm_input.frequency_hz && evse_pwm_input.frequency_hz <= 1010)
        return EVSE_CONNECTED;
    else if(hw_gpio_readPin(&n_evse_i_lim_pin))
        return WALL_CONNECTED;
    return DISCONNECTED;
}

void io_charger_inputCaptureCallback(TIM_HandleTypeDef *htim)
{
    assert(htim == evse_pwm_input.htim);
    hw_pwmInput_tick(&evse_pwm_input);
}