#include "hw_pwmOutputs.hpp"
#include <main.h>

const hw::PwmOutput led_dimming(&htim4, TIM_CHANNEL_4, 100.0f, PWM_TIMER_FREQUENCY);
const hw::PwmOutput seven_seg_dimming(&htim3, TIM_CHANNEL_1, 100.0f, PWM_TIMER_FREQUENCY);