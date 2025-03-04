#pragma once

#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_hal.h"
#include "hw_pwmInputFreqOnly.h"
#include "hw_adcs.h"

typedef struct
{
    PwmInputFreqOnly             *src;
    const PwmInputFreqOnlyConfig *config;
} CoolantFlowMeter;
#else
#include "app_utils.h"
EMPTY_STRUCT(CoolantFlowMeter);
#endif

void  io_coolant_init();
void  io_coolant_inputCaptureCallback();
void  io_coolant_checkIfFlowMeterActive();
float io_coolant_getFlowRate();

extern const CoolantFlowMeter coolant_flow_meter;
