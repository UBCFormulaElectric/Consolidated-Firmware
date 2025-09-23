#include "io_imd.h"
#include "hw_pwms.h"
#include "main.h"
#include "io_time.h"

#define PWM_TICKS_MAX 255

static uint8_t pwm_counter = 0;
/*
This is a software counter that is incremented everytime the status of the IMD
is broadcasted to CAN and the counter resets everytime the pwm signal from the
IMD is received and turned into an IMD state to broadcast. When the counter reaches
255 it will result in a 0 hz reading from the pwm signal. Usually if the IMD was not
sending a frequency, it would not trigger the interrupt that updates IMD state

// TODO there must be a smarter way to do this with timers
// NOTE: I am breaking this by removing the timer callback
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

float io_imd_getFrequency(void)
{
    return hw_pwmInput_getFrequency(&imd_pwm_input);
}

float io_imd_getDutyCycle(void)
{
    return hw_pwmInput_getDutyCycle(&imd_pwm_input);
}

uint32_t io_imd_getTimeSincePowerOn(void)
{
    // The IMD shares the same power rail as the BMS, so we assume that the IMD
    // and BMS boot up at the same time. In other words, the IMD has been ON
    // for just as long as the BMS.
    return io_time_getCurrentMs();
}
