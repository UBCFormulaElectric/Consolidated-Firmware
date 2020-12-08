#include "Io_LSM6DS33.h"

struct LSM6DS33
{
    // Acceleration in m/s^2
    float accel_x;
    float accel_y;
    float accel_z;
};

static struct LSM6DS33 lsm_6ds33;

void Io_LSM6DS33_ExtiCallback()
{
    lsm_6ds33.accel_x = 0.0f;
    lsm_6ds33.accel_y = 0.0f;
    lsm_6ds33.accel_z = 0.0f;
}

float Io_LSM6DS33_GetAccelerationX()
{
    return lsm_6ds33.accel_x;
}

float Io_LSM6DS33_GetAccelerationY()
{
    return lsm_6ds33.accel_y;
}

float Io_LSM6DS33_GetAccelerationZ()
{
    return lsm_6ds33.accel_z;
}
