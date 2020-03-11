#pragma once

#include <stm32f3xx_hal.h>
#include "SharedAssert.h"
#include <stdint.h>
#include <stdbool.h>
#include <arm_math.h>

typedef void *HallSensor_Handle;
typedef void *HallSensor_Settings_Handle;

typedef struct
{
    TIM_HandleTypeDef *htim;
    uint32_t           timer_frequency_hz;
    uint32_t           rising_edge_tim_channel;

} HallSensor_Settings_t;

typedef struct HallSensor
{
    HallSensor_Settings_t *settings;
    float32_t *            frequency_ptr;

} HallSensor_t;

/**
 * Shared function for initializing hall effect sensors
 * @param settings
 * @return
 */
HallSensor_Handle
    Shared_Init_Hall_Sensor(HallSensor_Settings_Handle *_settings);

/**
 * Shared function to update measured Hall Effect Sensor measured frequency
 * @param settings
 * @param HallSensor_Handle
 */

float32_t *Shared_Update_Freq_Hall_Sensor(HallSensor_Handle *_hall_sensor);
