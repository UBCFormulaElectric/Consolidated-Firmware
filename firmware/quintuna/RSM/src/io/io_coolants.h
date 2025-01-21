#pragma once

#include <stdbool.h>

#include "hw_hal.h"
#include "hw_pwmInputFreqOnly.h"
#include "hw_adcs.h"
#include "main.h"

void io_coolant_init(const PwmInputFreqOnlyConfig *config);

void io_coolant_inputCaptureCallback(void);

float io_coolant_getFlowRate(void);
void  io_coolant_checkIfFlowMeterActive(void);

bool io_coolant_temperature_ocsc(float v);

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
