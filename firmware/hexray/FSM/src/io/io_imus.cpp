extern "C"
{
#include "io_imu.h"
    ExitCode io_imu_init(void)
    {
        return EXIT_CODE_OK;
    }
    ExitCode io_imu_getLinearAccelerationX(float *x_acceleration)
    {
        return EXIT_CODE_OK;
    }
    ExitCode io_imu_getLinearAccelerationY(float *y_acceleration)
    {
        return EXIT_CODE_OK;
    }
    ExitCode io_imu_getLinearAccelerationZ(float *z_acceleration)
    {
        return EXIT_CODE_OK;
    }
    ExitCode io_imu_getAngularVelocityRoll(float *roll_velocity)
    {
        return EXIT_CODE_OK;
    }
    ExitCode io_imu_getAngularVelocityPitch(float *pitch_velocity)
    {
        return EXIT_CODE_OK;
    }
    ExitCode io_imu_getAngularVelocityYaw(float *yaw_velocity)
    {
        return EXIT_CODE_OK;
    }
}
