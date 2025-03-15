#include "hw_pwms.h"
#include "main.h"
#include "hw_pwmInput.h"

static const PwmInputConfig imd_pwm_input_config = {
    .htim                     = &htim1,
    .timer_frequency_hz       = TIM1_FREQUENCY / TIM1_PRESCALER,
    .rising_edge_tim_channel  = TIM_CHANNEL_1,
    .falling_edge_tim_channel = TIM_CHANNEL_2,
};

static const PwmInputConfig evse_pwm_input_config = {
    .htim                     = &htim15,
    .timer_frequency_hz       =  TIM5_FREQUENCY / TIM5_PRESCALER,
    .rising_edge_tim_channel  = TIM_CHANNEL_4,
    .falling_edge_tim_channel = TIM_CHANNEL_3
};