#pragma once

#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_hal.h"
#include "hw_pwmInputFreqOnly.h"
#include "hw_adcs.h"

typedef struct
{
    PwmInputFreqOnly       *src;
    PwmInputFreqOnlyConfig *config;
} CoolantFlowMeter;
#else
#include "app_utils.h"
EMPTY_STRUCT(CoolantFlowMeter);
#endif

void  io_coolant_init(CoolantFlowMeter *flow_meter);
void  io_coolant_inputCaptureCallback(CoolantFlowMeter *flow_meter);
void  io_coolant_checkIfFlowMeterActive(CoolantFlowMeter *flow_meter);
float io_coolant_getFlowRate(CoolantFlowMeter *flow_meter);

extern const CoolantFlowMeter coolant_flow_meter;
