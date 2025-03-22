#include "hw_pwms.h"
#include "main.h"

const PwmInputFreqOnlyConfig flow_meter_config = { .htim                = &htim4,
                                                   .tim_auto_reload_reg = TIM4_ARR,
                                                   .tim_channel         = TIM_CHANNEL_3,
                                                   .tim_frequency_hz    = TIMx_FREQUENCY / TIM4_PRESCALER,
                                                   .tim_active_channel  = HAL_TIM_ACTIVE_CHANNEL_3 };

const PwmInputFreqOnly flow_meter_5v5 = { .config = &flow_meter_config };