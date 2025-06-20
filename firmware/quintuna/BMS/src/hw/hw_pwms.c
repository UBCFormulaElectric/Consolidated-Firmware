#include "hw_pwms.h"
#include "main.h"
#include "hw_pwmInput.h"
#include "io_imd.h"
#include <assert.h>
#include <stdbool.h>

PwmInput imd_pwm_input = {
    .duty_cycle               = 0,
    .frequency_hz             = 0,
    .htim                     = &htim1,
    .timer_frequency_hz       = TIMx_FREQUENCY / TIM1_PRESCALER,
    .rising_edge_tim_channel  = TIM_CHANNEL_1,
    .falling_edge_tim_channel = TIM_CHANNEL_2,
};

PwmInput evse_pwm_input = { .duty_cycle               = 0,
                            .frequency_hz             = 0,
                            .htim                     = &htim5,
                            .timer_frequency_hz       = TIMx_FREQUENCY / TIM5_PRESCALER,
                            .rising_edge_tim_channel  = TIM_CHANNEL_4,
                            .falling_edge_tim_channel = TIM_CHANNEL_3 };

void hw_pwms_init(void)
{
    hw_pwmInput_init(&imd_pwm_input);
    hw_pwmInput_init(&evse_pwm_input);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim1)
    {
        hw_pwmInput_tick(&imd_pwm_input);
    }
    else if (htim == evse_pwm_input.htim)
    {
        hw_pwmInput_tick(&evse_pwm_input);
    }
    else
    {
        assert(false);
    }
}
