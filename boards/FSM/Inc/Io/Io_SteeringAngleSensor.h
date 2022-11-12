#pragma once

#include <stdbool.h>

/**
 * @return The voltage of the steering sensor
 */
float Io_SteeringSensorVoltage(void);
/**
 * Get the steering angle in degrees from the steering angle sensor
 * @return The steering angle of the steering angle sensor in degrees
 */
float Io_SteeringAngleSensor_GetAngleDegree(void);
/**
 * @return whether or not the steering sensor is open or short circuit
 */
bool Io_SteeringSensorOCSC(void);
