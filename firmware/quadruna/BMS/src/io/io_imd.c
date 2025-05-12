#include "io_imd.h"
#include <assert.h>
#include "main.h"
#include "hw_pwmInput.h"
#include "io_time.h"

#define PWM_TICKS_MAX 255

static PwmInput imd_pwm_input = {
    .duty_cycle               = 0,
    .frequency_hz             = 0,
    .htim                     = &htim1,
    .timer_frequency_hz       = TIM1_FREQUENCY / TIM1_PRESCALER,
    .rising_edge_tim_channel  = TIM_CHANNEL_1,
    .falling_edge_tim_channel = TIM_CHANNEL_2,
};

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

void io_imd_init(void)
{
    hw_pwmInput_init(&imd_pwm_input);
}

float io_imd_getFrequency(void)
{
    return hw_pwmInput_getFrequency(&imd_pwm_input);
}

float io_imd_getDutyCycle(void)
{
    return hw_pwmInput_getDutyCycle(&imd_pwm_input);
}

void io_imd_inputCaptureCallback(TIM_HandleTypeDef *htim)
{
    assert(htim == imd_pwm_input.htim);

    hw_pwmInput_tick(&imd_pwm_input);
    pwm_counter = 0; // Reset the ticks since the last pwm reading
}

uint32_t io_imd_getTimeSincePowerOn(void)
{
    // The IMD shares the same power rail as the BMS, so we assume that the IMD
    // and BMS boot up at the same time. In other words, the IMD has been ON
    // for just as long as the BMS.
    return io_time_getCurrentMs();
}
