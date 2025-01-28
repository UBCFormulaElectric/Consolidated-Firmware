#pragma once
#include <stdbool.h>
#include <hw_adcs.h>

typedef struct
{
    const AdcChannel *src;
} Suspension;

float io_suspension_getTravel(const Suspension *suspension);
bool  io_suspension_OCSC(const Suspension *suspension);

extern const Suspension right;
extern const Suspension left;