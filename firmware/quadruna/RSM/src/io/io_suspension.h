#pragma once
#include <stdbool.h>

/**
 * Get the travel of the rear left suspension
 * @return The travel of the suspension in (TODO: add units)
 */

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