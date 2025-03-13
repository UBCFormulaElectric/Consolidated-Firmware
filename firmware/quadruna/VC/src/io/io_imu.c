#include "io_imu.h"
#include "hw_i2cs.h"
bool io_imu_init()
{
    return io_shared_imu_init(&imu_i2c);
}


bool io_imu_getLinearAccelerationX(float *x_acceleration)
{
    return io_shared_imu_getLinearAccelerationX(&imu_i2c, x_acceleration);
}

bool io_imu_getLinearAccelerationY(float *y_acceleration)
{
    return io_shared_imu_getLinearAccelerationY(&imu_i2c, y_acceleration);
}

bool io_imu_getLinearAccelerationZ(float *z_acceleration)
{
    return io_shared_imu_getLinearAccelerationZ(&imu_i2c, z_acceleration);
}

bool io_imu_getAngularVelocityRoll(float *roll_velocity)
{
    return io_shared_imu_getAngularVelocityRoll(&imu_i2c, roll_velocity);
}

bool io_imu_getAngularVelocityPitch(float *pitch_velocity)
{
    return io_shared_imu_getAngularVelocityPitch(&imu_i2c, pitch_velocity);
}

bool io_imu_getAngularVelocityYaw(float *yaw_velocity)
{
    return io_shared_imu_getAngularVelocityYaw(&imu_i2c, yaw_velocity);
}
