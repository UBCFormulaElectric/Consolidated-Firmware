#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <App_SharedExitCode.h>

struct Imu;

/**
 * Allocate and initialize an IMU.
 * @param update_sensor_data function which updates data and returns
 * EXIT_CODE_OK if successful, EXIT_CODE_OUT_OF_RANGE if data is not within
 * specified range, EXIT_CODE_INVALID_ARGS if data not read successfully
 * @param get_acceleration_x A function that can be called to get the
 * acceleration (m/s^2) on the x-axis.
 * @param get_acceleration_y A function that can be called to get the
 * acceleration (m/s^2) on the y-axis.
 * @param get_acceleration_z A function that can be called to get the
 * acceleration (m/s^2) on the z-axis.
 * @param min_acceleration The min acceleration the Imu should read for a given
 * axis, in m/s^2
 * @param max_acceleration The max acceleration the Imu should read for a given
 * axis, in m/s^2
 * @return The created IMU whose ownership is given to the caller.
 */
struct Imu *App_Imu_Create(
    ExitCode (*update_sensor_data)(void),
    float (*get_acceleration_x)(void),
    float (*get_acceleration_y)(void),
    float (*get_acceleration_z)(void),
    float min_acceleration,
    float max_acceleration);

/**
 * Deallocate the memory used by the given Imu
 * @param imu The Imu to deallocate
 */
void App_Imu_Destroy(struct Imu *imu);

/**
 * Update acceleration and angular rate values for the given Imu.
 * @param imu The Imu to update the acceleration and angular rate values for.
 * @return ExitCode which is EXIT_CODE_OK if successful, EXIT_CODE_OUT_OF_RANGE
 * if data is not within specified range, EXIT_CODE_INVALID_ARGS if data not
 * read successfully
 */
ExitCode App_Imu_UpdateSensorData(const struct Imu *const imu);

/**
 * Get acceleration (m/s^2) measured on the x-axis from the given Imu.
 * @param imu The Imu to get the x-axis acceleration from.
 * @return The acceleration (m/s^2) measured on the x-axis.
 */
float App_Imu_GetAccelerationX(const struct Imu *imu);

/**
 * Get acceleration (m/s^2) measured on the y-axis from the given Imu.
 * @param imu The Imu to get the y-axis acceleration from.
 * @return The acceleration (m/s^2) measured on the y-axis.
 */
float App_Imu_GetAccelerationY(const struct Imu *imu);

/**
 * Get acceleration (m/s^2) measured on the z-axis from the given Imu.
 * @param The Imu to get the z-axis acceleration from.
 * @return The acceleration (m/s^2) measured on the z-axis.
 */
float App_Imu_GetAccelerationZ(const struct Imu *imu);

/**
 * Get InRangeCheck corresponding to acceleration on the x-axis for the given
 * Imu
 * @param imu The Imu to get the InRangeCheck from.
 * @return InRangeCheck corresponding to acceleration on the x-axis for the
 * given Imu
 */
struct InRangeCheck *
    App_Imu_GetAccelerationXInRangeCheck(const struct Imu *imu);

/**
 * Get InRangeCheck corresponding to acceleration on the y-axis for the given
 * Imu
 * @param imu The Imu to get the InRangeCheck from.
 * @return The InRangeCheck corresponding to acceleration on the y-axis for the
 * given Imu
 */
struct InRangeCheck *
    App_Imu_GetAccelerationYInRangeCheck(const struct Imu *imu);

/**
 * Get InRangeCheck corresponding to acceleration on the z-axis for the given
 * Imu
 * @param imu The Imu to get the InRangeCheck from.
 * @return InRangeCheck corresponding to acceleration on the z-axis for the
 * given Imu
 */
struct InRangeCheck *
    App_Imu_GetAccelerationZInRangeCheck(const struct Imu *imu);
