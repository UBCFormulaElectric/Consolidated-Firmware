#pragma once
#include <stdbool.h>

/**
 * Get the steering angle in degrees from the steering angle sensor
 * @return The steering angle of the steering angle sensor in degrees
 */
float io_steering_getAngleDegrees(void);

/**
 * @return whether or not the steering sensor is open or short circuit
 */
bool io_steering_sensorOCSC(void);
