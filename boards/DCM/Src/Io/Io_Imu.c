#include "Io_Imu.h"
#include <stm32f3xx_hal.h>

struct LSM6DS33
{
    // Acceleration in m/s^2
    float accel_x;
    float accel_y;
    float accel_z;
};

static struct LSM6DS33 lsm_6ds33;

float Io_Imu_GetAccelerationX()
{
    return lsm_6ds33.accel_x;
}

float Io_Imu_GetAccelerationY()
{
    return lsm_6ds33.accel_y;
}

float Io_Imu_GetAccelerationZ()
{
    return lsm_6ds33.accel_z;
}
