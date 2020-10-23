#pragma once
#include <stdint.h>
#include <stdbool.h>

struct Imu;

struct ImuData
{
    // Acceleration in m/s^2
    float accel_x;
    float accel_y;
    float accel_z;

    // The time (in ms) that this data was received
    uint32_t received_time_ms;
};

/** Creates Imu struct used for reading data from sensor
 * @param update_data - pointer to function which updates data and returns true if update successful
 * @param get_imu_data - pointer to function which returns Imu data in ImuData struct
 * @return Imu data in ImuData struct
 */
struct Imu *App_Imu_Create(
    bool (*update_data)(void),
    struct ImuData *(*get_imu_data)(void));

/** Update stored Imu values
 * @param imu - pointer to Imu struct
 * @return true if update successful & data is valid, false otherwise
 */
bool  App_Imu_UpdateData(const struct Imu *imu);

/** Get most recent x acceleration reading from Imu
 * @param imu
 * @return x acceleration in m/s^2
 */
float App_Imu_GetAccelerationX(const struct Imu *imu);

/** Get most recent y acceleration reading from Imu
 * @param imu
 * @return y acceleration in m/s^2
 */
float App_Imu_GetAccelerationY(const struct Imu *imu);

/** Get most recent z acceleration reading from Imu
 * @param imu
 * @return z acceleration in m/s^2
 */
float App_Imu_GetAccelerationZ(const struct Imu *imu);
