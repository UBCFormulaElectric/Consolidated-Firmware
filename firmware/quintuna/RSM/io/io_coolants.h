#pragma once

#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_hal.h"
#include "hw_pwmInputFreqOnly.h"
#include "hw_adcs.h"

void io_coolant_init(void);

void io_coolant_inputCaptureCallback(void);

float io_coolant_getFlowRate(void);
void  io_coolant_checkIfFlowMeterActive(void);

bool  io_coolant_temperature_ocsc(float v);
float io_coolant_getTemperature(void);

bool  io_coolant_pressure_ocsc(float v);
float io_coolant_getPressureA(void);
float io_coolant_getPressureB(void);

bool io_coolant_PressureAOCSC(void);
bool io_coolant_pressureBOCSC(void);

typedef struct
{
    Adc *src;
} Coolant;

float io_coolant_getPressure(Coolant *coolant);
float io_coolant_getTemperature(Coolant *coolant)

extern const Coolant a;
extern const Coolant b;
