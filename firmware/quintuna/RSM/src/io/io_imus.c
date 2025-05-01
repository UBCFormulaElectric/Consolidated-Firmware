#include "io_imus.h"
#include "hw_i2cs.h"
#include "io_imu.h"

// TODO: set offset values
const imuConfig imu_config = { .imu_i2c_handle = &imu_i2c,
                               .x_accel_offset = 0.0f,
                               .y_accel_offset = 0.0f,
                               .z_accel_offset = 0.0f,
                               .roll_offset    = 0.0f,
                               .pitch_offset   = 0.0f,
                               .yaw_offset     = 0.0f };

bool io_imu_getLinAccel(float *x, float *y, float *z)
{
    if (io_imu_getLinearAccelerationX(x) && io_imu_getLinearAccelerationY(y) && io_imu_getLinearAccelerationZ(z))
    {
        return true;
    }
    return false;
}

bool io_imu_getAngVel(float *x, float *y, float *z)
{
    if (io_imu_getAngularVelocityRoll(x) && io_imu_getAngularVelocityPitch(y) && io_imu_getAngularVelocityYaw(z))
    {
        return true;
    }
    return false;
}