#pragma once

#include <stm32f3xx_hal.h>

#include "Io_SharedFrequencyOnlyPwmInputConfig.h"

#define MAX_NUM_OF_FREQ_ONLY_PWM_INPUT 2

#ifndef MAX_NUM_OF_FREQ_ONLY_PWM_INPUT
#error Missing definition: MAX_NUM_OF_FREQ_ONLY_PWM_INPUTS must be defined in Io_SharedFreqOnlyPwmInputConfig.h.
#endif

struct FrequencyOnlyPwmInput;

struct FrequencyOnlyPwmInput *Io_SharedFrequencyOnlyPwmInput_Create(
    TIM_HandleTypeDef *htim,
    float              timer_frequency_hz,
    uint32_t           rising_edge_tim_channel);

float Io_SharedFrequencyOnlyPwmInput_GetFrequency(
    struct FrequencyOnlyPwmInput *const frequency_only_pwm_input);

void Io_SharedFrequencyOnlyPwmInput_Tick(
    struct FrequencyOnlyPwmInput *const frequency_only_pwm_input);
