#include "io_imuFake.h"

namespace fakes::io_imu
{
static bool initialized = false;
void        reset_init()
{
    initialized = false;
}
bool get_init()
{
    return initialized;
}

static float x_acceleration = 0.0f;
void         set_LinearAccelerationX(const float x_acceleration)
{
    // Set the linear acceleration in the x direction
    io_imu::x_acceleration = x_acceleration;
}

static float y_acceleration = 0.0f;
void         set_LinearAccelerationY(const float y_acceleration)
{
    // Set the linear acceleration in the y direction
    io_imu::y_acceleration = y_acceleration;
}

static float z_acceleration = 0.0f;
void         set_LinearAccelerationZ(const float z_acceleration)
{
    // Set the linear acceleration in the z direction
    io_imu::z_acceleration = z_acceleration;
}

static float roll_velocity = 0.0f;
void         set_AngularVelocityRoll(const float roll_velocity)
{
    // Set the angular velocity in the roll direction
    io_imu::roll_velocity = roll_velocity;
}

static float pitch_velocity = 0.0f;
void         set_AngularVelocityPitch(const float pitch_velocity)
{
    // Set the angular velocity in the pitch direction
    io_imu::pitch_velocity = pitch_velocity;
}

static float yaw_velocity = 0.0f;
void         set_AngularVelocityYaw(const float yaw_velocity)
{
    // Set the angular velocity in the yaw direction
    io_imu::yaw_velocity = yaw_velocity;
}
} // namespace fakes::io_imu

extern "C"
{
#include "io_imu.h"
    ExitCode io_imu_init(void)
    {
        fakes::io_imu::initialized = true;
        return EXIT_CODE_OK;
    }
    ExitCode io_imu_getLinearAccelerationX(float *x_acceleration)
    {
        *x_acceleration = fakes::io_imu::x_acceleration;
        return EXIT_CODE_OK;
    }
    ExitCode io_imu_getLinearAccelerationY(float *y_acceleration)
    {
        *y_acceleration = fakes::io_imu::y_acceleration;
        return EXIT_CODE_OK;
    }
    ExitCode io_imu_getLinearAccelerationZ(float *z_acceleration)
    {
        *z_acceleration = fakes::io_imu::z_acceleration;
        return EXIT_CODE_OK;
    }
    ExitCode io_imu_getAngularVelocityRoll(float *roll_velocity)
    {
        *roll_velocity = fakes::io_imu::roll_velocity;
        return EXIT_CODE_OK;
    }
    ExitCode io_imu_getAngularVelocityPitch(float *pitch_velocity)
    {
        *pitch_velocity = fakes::io_imu::pitch_velocity;
        return EXIT_CODE_OK;
    }
    ExitCode io_imu_getAngularVelocityYaw(float *yaw_velocity)
    {
        *yaw_velocity = fakes::io_imu::yaw_velocity;
        return EXIT_CODE_OK;
    }
}
