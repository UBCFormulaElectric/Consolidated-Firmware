#pragma once
#include "hw_adcs.h"
#include <stdbool.h>

/**
 * Get the travel of the rear left suspension
 * @return The travel of the suspension in (TODO: add units)
 */

namespace io::suspension
{

float getRearLeftTravel(void);

/**
 * Get the travel of the rear right suspension
 * @return The travel of the suspension in (TODO: add units)
 */
float getRearRightTravel(void);

/**
 * @return whether or not the left suspension sensor is open or short circuit
 */
bool leftSensorOCSC(void);

/**
 * @return whether or not the right suspension sensor is open or short circuit
 */
bool rightSensorOCSC(void);

class SuspensionSensor
{
    hw::Adc *src;

  public:
    SuspensionSensor(hw::Adc *src) { this->src = src; }

    float getTravel();
    float OCSC();
};

static extern const SuspensionSensor right;
static extern const SuspensionSensor left;

} // namespace io::suspension
