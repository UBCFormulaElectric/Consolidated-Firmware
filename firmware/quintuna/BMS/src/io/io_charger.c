#include "hw_gpios.h"
#include "hw_pwms.h"
#include "io_charger.h"
#include "cmsis_os.h"
#include "assert.h"

ConnectionStatus io_charger_getConnectionStatus()
{
    if(990 <= evse_pwm_input.frequency_hz && evse_pwm_input.frequency_hz <= 1010)
    {
        return EVSE_CONNECTED;
    }
    return EVSE_DISCONNECTED;
}

void io_charger_inputCaptureCallback(TIM_HandleTypeDef *htim)
{
    assert(htim == evse_pwm_input.htim);
    hw_pwmInput_tick(&evse_pwm_input);
}