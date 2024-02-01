#include "io_imd.h"
#include <assert.h>
#include "main.h"
#include "hw_pwmInput.h"
#include "io_time.h"

static PwmInput pwm_input;

uint8_t io_pwm_counter_tick(void)
{
    return hw_pwm_counter_tick();
}

void io_imd_init(const PwmInputConfig *pwm_input_config)
{
    io_pwmInput_init(&pwm_input, pwm_input_config);
}

float io_imd_getFrequency(void)
{
    return hw_pwmInput_getFrequency(&pwm_input);
}

float io_imd_getDutyCycle(void)
{
    return hw_pwmInput_getDutyCycle(&pwm_input);
}

void io_imd_inputCaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim == pwm_input.config->htim)
    {
        hw_pwmInput_tick(&pwm_input);
    }
}

uint32_t io_imd_getTimeSincePowerOn(void)
{
    // The IMD shares the same power rail as the BMS, so we assume that the IMD
    // and BMS boot up at the same time. In other words, the IMD has been ON
    // for just as long as the BMS.
    return io_time_getCurrentMs();
}
