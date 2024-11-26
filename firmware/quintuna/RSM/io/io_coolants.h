#pragma once

#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_hal.h"
#include "hw_pwmInputFreqOnly.h"
#include "hw_adcs.h"
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

class Coolant
{
    hw::Adc *src;

  public:
    float getPressure();

    Coolant(hw::Adc *src) { this->src = src; }
} static extern const Coolant a;
static extern const Coolant   b;
} // namespace io::coolant
