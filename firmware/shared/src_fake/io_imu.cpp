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

static float _x_acceleration = 0.0f;
void         set_LinearAccelerationX(const float x_acceleration)
{
    // Set the linear acceleration in the x direction
    _x_acceleration = x_acceleration;
}

static float _y_acceleration = 0.0f;
void         set_LinearAccelerationY(const float y_acceleration)
{
    // Set the linear acceleration in the y direction
    _y_acceleration = y_acceleration;
}

static float _z_acceleration = 0.0f;
void         set_LinearAccelerationZ(const float z_acceleration)
{
    // Set the linear acceleration in the z direction
    _z_acceleration = z_acceleration;
}

static float _roll_velocity = 0.0f;
void         set_AngularVelocityRoll(const float roll_velocity)
{
    // Set the angular velocity in the roll direction
    _roll_velocity = roll_velocity;
}

static float _pitch_velocity = 0.0f;
void         set_AngularVelocityPitch(const float pitch_velocity)
{
    // Set the angular velocity in the pitch direction
    _pitch_velocity = pitch_velocity;
}

static float _yaw_velocity = 0.0f;
void         set_AngularVelocityYaw(const float yaw_velocity)
{
    // Set the angular velocity in the yaw direction
    _yaw_velocity = yaw_velocity;
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
        *x_acceleration = fakes::io_imu::_x_acceleration;
        return EXIT_CODE_OK;
    }
    ExitCode io_imu_getLinearAccelerationY(float *y_acceleration)
    {
        *y_acceleration = fakes::io_imu::_y_acceleration;
        return EXIT_CODE_OK;
    }
    ExitCode io_imu_getLinearAccelerationZ(float *z_acceleration)
    {
        *z_acceleration = fakes::io_imu::_z_acceleration;
        return EXIT_CODE_OK;
    }
    ExitCode io_imu_getAngularVelocityRoll(float *roll_velocity)
    {
        *roll_velocity = fakes::io_imu::_roll_velocity;
        return EXIT_CODE_OK;
    }
    ExitCode io_imu_getAngularVelocityPitch(float *pitch_velocity)
    {
        *pitch_velocity = fakes::io_imu::_pitch_velocity;
        return EXIT_CODE_OK;
    }
    ExitCode io_imu_getAngularVelocityYaw(float *yaw_velocity)
    {
        *yaw_velocity = fakes::io_imu::_yaw_velocity;
        return EXIT_CODE_OK;
    }
}