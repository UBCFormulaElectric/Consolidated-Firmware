#include "hw_gpios.h"
#include "hw_pwms.h"
#include "io_charger.h"
#include "cmsis_os.h"
#include "assert.h"

void io_charger_init(void)
{
    hw_gpio_writePin(&n_evse_i_lim_pin, true);
}

void io_charger_vehiclePresent(void)
{
    io_charger_init();
}

void io_charger_requestCharge(void)
{
    hw_gpio_writePin(&n_evse_i_lim_pin, false);
}

void io_charger_inputCaptureCallback(TIM_HandleTypeDef *htim)
{
    assert(htim == evse_pwm_input.htim);

    hw_pwmInput_tick(&evse_pwm_input);
}