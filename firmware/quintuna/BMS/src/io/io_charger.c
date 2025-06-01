#include "hw_gpios.h"
#include "io_charger.h"
#include "cmsis_os.h"

#define PWM_TIMEOUT_MS 1000.0f

EvseStatus io_charger_getStatus(void)
{
    EvseStatus current_status = EVSE_DISCONNECTED;

    if (hw_gpio_readPin(&n_evse_i_lim_pin))
    {
        current_status = EVSE_CONNECTED;
    }

    return current_status;
}

void io_charger_pwm_callback(TIM_HandleTypeDef *htim) {}
