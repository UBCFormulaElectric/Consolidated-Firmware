#pragma once
#include <stdbool.h>
#include <hw_adcs.h>

extern float travelFromVoltage(float voltage);

typedef struct
{
    const AdcChannel *src;
} Suspension;

float io_suspension_getTravel(const Suspension *suspension);

bool io_suspension_SensorOCSC(const Suspension *suspension);

extern const Suspension right;
extern const Suspension left;