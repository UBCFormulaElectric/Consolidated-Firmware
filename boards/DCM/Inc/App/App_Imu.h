#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <App_SharedExitCode.h>
#include "App_InRangeCheck.h"

#define MIN_ACCELERATION_MS2 -30.0f
#define MAX_ACCELERATION_MS2 30.0f

/**
 * Initialize IMU module.
 */
void App_Imu_Init();

/**
 * Get the acceleration measured on the x-axis from the given Imu
 * @return The acceleration (m/s^2) measured on the x-axis.
 */
float App_Imu_GetAccelerationX();

/**
 * Get acceleration measured on the y-axis from the given Imu
 * @return The acceleration (m/s^2) measured on the y-axis.
 */
float App_Imu_GetAccelerationY();

/**
 * Get acceleration measured on the z-axis from the given Imu
 * @return The acceleration (m/s^2) measured on the z-axis.
 */
float App_Imu_GetAccelerationZ();

/**
 * Get the acceleration-x in-range check from the given Imu
 * @return The acceleration-x in-range check for the given Imu
 */
InRangeCheck *App_Imu_GetAccelerationXInRangeCheck();

/**
 * Get the acceleration-y in-range check from the given Imu
 * @return The acceleration-y in-range check for the given Imu
 */
InRangeCheck *App_Imu_GetAccelerationYInRangeCheck();

/**
 * Get the acceleration-z in-range check from the given Imu
 * @return The acceleration-z in-range check for the given Imu
 */
InRangeCheck *App_Imu_GetAccelerationZInRangeCheck();
