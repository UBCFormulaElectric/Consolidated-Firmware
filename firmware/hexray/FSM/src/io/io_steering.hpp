#pragma once
#include <stdbool.h>

namespace io::steering
{
/**
 * Get the steering angle in degrees from the steering angle sensor
 * @return The steering angle of the steering angle sensor in degrees
 */
float getAngleDegrees(void);

/**
 * @return whether the steering sensor is open or short circuit
 */
bool sensorOCSC(void);
} // namespace io::steering
