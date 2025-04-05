#include "hw_gpios.h"
#include "io_charger.h"
#include "cmsis_os.h"

#define PWM_TIMEOUT_MS 1000.0f

EVSE_STATUS io_charger_get_status(void) {

    EVSE_STATUS current_status = EVSE_DISCONNECTED;

    if(hw_gpio_readPin(&n_evse_i_lim_pin)) {

        current_status = EVSE_CONNECTED;
    }

    return current_status;
}

void io_charger_pwm_callback(TIM_HandleTypeDef *htim) {    
}
