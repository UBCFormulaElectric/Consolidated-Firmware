#pragma once

#include <stm32f3xx_hal.h>
#include "SharedAssert.h"
#include "stdint.h"

struct Hall_Sensor_Input_Settings{

  TIM_HandleTypeDef *htim;
  uint32_t timer_frequency_hz;
  uint32_t rising_edge_tim_channel;

};

struct Hall_Sensor_Input{

    struct Hall_Sensor_Input_Settings settings;
    float * frequency_ptr;

};

void Shared_Hall_Sensor_Init (
        struct Hall_Sensor_Input * _hall_sensor_input, struct Hall_Sensor_Input_Settings, float * frequency_ptr);

