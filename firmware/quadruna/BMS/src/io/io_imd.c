#include "io_imd.h"
#include <assert.h>
#include "main.h"
#include "hw_pwmInput.h"
#include "io_time.h"

#define PWM_TICKS_MAX 255

static PwmInput pwm_input;

static uint8_t pwm_counter = 0;
/*
This is a software counter that is incremented everytime the status of the IMD
is broadcasted to CAN and the counter resets everytime the pwm signal from the
IMD is received and turned into an IMD state to broadcast. When the counter reaches
255 it will result in a 0 hz reading from the pwm signal. Usually if the IMD was not
sending a frequency, it would not trigger the interrupt that updates IMD state
*/

uint8_t io_imd_pwmCounterTick(void)
{
    if (pwm_counter != PWM_TICKS_MAX)
    {
        pwm_counter++;
        return pwm_counter;
    }
    return pwm_counter;
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
        pwm_counter = 0; // Reset the ticks since the last pwm reading
    }
}

uint32_t io_imd_getTimeSincePowerOn(void)
{
    // The IMD shares the same power rail as the BMS, so we assume that the IMD
    // and BMS boot up at the same time. In other words, the IMD has been ON
    // for just as long as the BMS.
    return io_time_getCurrentMs();
}
