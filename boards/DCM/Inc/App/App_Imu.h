#pragma once
#include <stdint.h>
#include <stdbool.h>

struct Imu;

/** Creates Imu struct used for reading data from sensor
 * @param update_data - pointer to function which updates data and returns true
 * if update successful
 * @param get_acceleration_x - pointer to function which returns x acceleration in m/s^2
 * @param get_acceleration_y - pointer to function which returns y acceleration in m/s^2
 * @param get_acceleration_z - pointer to function which returns z acceleration in m/s^2
 * struct
 * @return Imu data in ImuData struct
 */
struct Imu *App_Imu_Create(
    bool (*update_data)(void),
    float (*get_acceleration_x)(void),
    float (*get_acceleration_y)(void),
    float (*get_acceleration_z)(void));

/** Update stored Imu values
 * @param imu - pointer to Imu struct
 * @return true if update successful & data is valid, false otherwise
 */
bool App_Imu_UpdateData(const struct Imu *imu);

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
