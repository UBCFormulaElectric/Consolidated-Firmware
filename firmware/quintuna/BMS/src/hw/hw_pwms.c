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
    .timer_frequency_hz       = TIM1_FREQUENCY / TIM1_PRESCALER,
    .rising_edge_tim_channel  = TIM_CHANNEL_1,
    .falling_edge_tim_channel = TIM_CHANNEL_2,
};

PwmInput evse_pwm_input = {   
    .duty_cycle               = 0,
    .frequency_hz             = 0,
    .htim                     = &htim5,
    .timer_frequency_hz       = TIM5_FREQUENCY / TIM5_PRESCALER,
    .rising_edge_tim_channel  = TIM_CHANNEL_4,
    .falling_edge_tim_channel = TIM_CHANNEL_3 
};

void hw_pwms_init(void){
    hw_pwmInput_init(&imd_pwm_input);
    hw_pwmInput_init(&evse_pwm_input);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim1){
        io_imd_inputCaptureCallback(htim);
    }
    else if(htim == &htim5){
        // io_evse_inputCaptureCallback(htim)
    }
    else{
        assert(false);
    }
}
                                                      