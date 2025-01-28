#pragma once

#include <stdbool.h>

#include "hw_hal.h"
#include "hw_pwmInputFreqOnly.h"
#include "hw_adcs.h"
#include "main.h"

typedef struct
{
    const AdcChannel *src;
} Coolant;

float io_coolant_getPressure(const Coolant *coolant);
float io_coolant_getTemperature(const Coolant *coolant);
bool  io_coolant_pressureOCSC(const Coolant *coolant);
bool  io_coolant_temperatureOCSC(const Coolant *coolant);

extern const Coolant a;
extern const Coolant b;

typedef struct
{
    const PwmInputFreqOnly       *src;
    const PwmInputFreqOnlyConfig *config;

} CoolantFlowMeter;

void  io_coolant_init(PwmInputFreqOnlyConfig *config, CoolantFlowMeter *coolant_flow);
void  io_coolant_inputCaptureCallback(CoolantFlowMeter *coolant_flow);
void  io_coolant_checkIfFlowMeterActive(CoolantFlowMeter *coolant_flow);
float io_coolant_getFlowRate(CoolantFlowMeter *coolant_flow);

extern const CoolantFlowMeter coolant_flow_meter;
