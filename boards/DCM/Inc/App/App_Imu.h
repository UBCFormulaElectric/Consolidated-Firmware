#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <App_SharedExitCode.h>

struct Imu;

/**
 * Allocate and initialize an Imu.
 * @param get_acceleration_x A function that can be called to get the
 * acceleration (m/s^2) on the x-axis.
 * @param get_acceleration_y A function that can be called to get the
 * acceleration (m/s^2) on the y-axis.
 * @param get_acceleration_z A function that can be called to get the
 * acceleration (m/s^2) on the z-axis.
 * @param min_acceleration The minimum acceleration (m/s^2) measurable by the
 * Imu for any given axis
 * @param max_acceleration The maximum acceleration (m/s^2) measurable by the
 * Imu for any given axis
 * @return The created Imu whose ownership is given to the caller.
 */
struct Imu *App_Imu_Create(
    float (*get_acceleration_x)(void),
    float (*get_acceleration_y)(void),
    float (*get_acceleration_z)(void),
    float min_acceleration,
    float max_acceleration);

/**
 * Deallocate the memory used by the given Imu
 * @param imu The given Imu to deallocate
 */
void App_Imu_Destroy(struct Imu *imu);

/**
 * Get the acceleration measured on the x-axis from the given Imu
 * @param imu The given Imu to get the x-axis acceleration from.
 * @return The acceleration (m/s^2) measured on the x-axis.
 */
float App_Imu_GetAccelerationX(const struct Imu *imu);

/**
 * Get acceleration measured on the y-axis from the given Imu
 * @param imu The given Imu to get the y-axis acceleration from.
 * @return The acceleration (m/s^2) measured on the y-axis.
 */
float App_Imu_GetAccelerationY(const struct Imu *imu);

/**
 * Get acceleration measured on the z-axis from the given Imu
 * @param The given Imu to get the z-axis acceleration from.
 * @return The acceleration (m/s^2) measured on the z-axis.
 */
float App_Imu_GetAccelerationZ(const struct Imu *imu);

/**
 * Get the acceleration-x in-range check from the given Imu
 * @param imu The given Imu to get the acceleration-x in-range check from
 * @return The acceleration-x in-range check for the given Imu
 */
struct InRangeCheck *App_Imu_GetAccelerationXInRangeCheck(const struct Imu *imu);

/**
 * Get the acceleration-y in-range check from the given Imu
 * @param imu The given Imu to get the acceleration-y in-range check from
 * @return The acceleration-y in-range check for the given Imu
 */
struct InRangeCheck *App_Imu_GetAccelerationYInRangeCheck(const struct Imu *imu);

/**
 * Get the acceleration-z in-range check from the given Imu
 * @param imu The given Imu to get the acceleration-z in-range check from
 * @return The acceleration-z in-range check for the given Imu
 */
struct InRangeCheck *App_Imu_GetAccelerationZInRangeCheck(const struct Imu *imu);
