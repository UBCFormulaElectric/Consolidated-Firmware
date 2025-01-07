#pragma once
#include "hw_adcs.h"
#include <stdbool.h>

/**
 * Get the travel of the rear left suspension
 * @return The travel of the suspension in (TODO: add units)
 */

typedef struct
{
    Adc *src;
} SuspensionSensor;

float getTravel(SuspensionSensor *suspension);
float OCSC(SuspensionSensor *suspension);

static extern const SuspensionSensor right;
static extern const SuspensionSensor left;
