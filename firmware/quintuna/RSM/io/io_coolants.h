#pragma once

#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_hal.h"
#include "hw_pwmInputFreqOnly.h"
namespace io::coolant
{
void init(void);

void inputCaptureCallback(void);

float getFlowRate(void);
void  checkIfFlowMeterActive(void);

bool  temperature_ocsc(float v);
float getTemperature(void);

bool  pressure_ocsc(float v);
float getPressureA(void);
float getPressureB(void);

bool pressureBOCSC(void);
} // namespace io::coolant
