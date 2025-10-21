#include "hw/pwms.h"
#include "main.h"

PwmOutput seven_seg_dimming = { .htim         = &htim4,
                                .pwm_channel  = TIM_CHANNEL_1,
                                .duty_cycle   = 0.0f,
                                .frequency_hz = (SYS_TIMER_CLOCK / ((PWM_PRESCALER + 1) * (PWM_AUTO_RELOAD))) };
PwmOutput led_dimming       = { .htim         = &htim12,
                                .pwm_channel  = TIM_CHANNEL_1,
                                .duty_cycle   = 0.0f,
                                .frequency_hz = (SYS_TIMER_CLOCK / ((PWM_PRESCALER + 1) * (PWM_AUTO_RELOAD))) };