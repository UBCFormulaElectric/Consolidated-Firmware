#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <SharedAssert.h>
#include <Shared_Hall_Sensor.h>

typedef void *FlowMeter_Handle;

/**
 * @brief Calculate the flow rate (L/min) from the measured freq. from
 *        Hall Effect Sensor
 * @param FlowMeter_Handle
 * @return
 */
float32_t *_Get_Flow_Rate(HallSensor_Handle *FlowMeter_Handle);

/**
 * @brief Initializes the Flow Meter Struct
 * @return
 */
FlowMeter_Handle *_Init_FlowMeter(void);

float32_t _Update_Flow_Rate(FlowMeter_Handle *_Flow_Meter);

void _show_struct_content(FlowMeter_Handle *_flow_meter);
