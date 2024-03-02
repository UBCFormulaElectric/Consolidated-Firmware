#pragma once
#include <stdbool.h>

/**
 * Get the travel of the front left suspension
 * @return The travel of the suspension in (TODO: add units)
 */
float io_suspension_getLeftTravel(void);

/**
 * Get the travel of the front right suspension
 * @return The travel of the suspension in (TODO: add units)
 */
float io_suspension_getRightTravel(void);

/**
 * @return whether or not the left suspension sensor is open or short circuit
 */
bool io_suspension_leftSensorOCSC(void);

/**
 * @return whether or not the right suspension sensor is open or short circuit
 */
bool io_suspension_rightSensorOCSC(void);