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

// TODO: Comment these
struct Imu *App_Imu_Create(
    bool (*update_data)(void),
    struct ImuData *(*get_imu_data)(void));
bool  App_Imu_UpdateData(const struct Imu *imu);
float App_Imu_GetAccelerationX(const struct Imu *imu);
float App_Imu_GetAccelerationY(const struct Imu *imu);
float App_Imu_GetAccelerationZ(const struct Imu *imu);
