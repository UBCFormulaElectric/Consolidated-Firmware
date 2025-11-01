#include "app_imu.hpp"

extern "C"
{
    #include "io_imu.h"
    #include "app_canTx.h"
}

namespace io::imu
{
    ExitCode getLinearAccelerationX(float *value);
    ExitCode getLinearAccelerationY(float *value);
    ExitCode getLinearAccelerationZ(float *value);
    ExitCode getAngularVelocityRoll(float *value);
    ExitCode getAngularVelocityPitch(float *value);
    ExitCode getAngularVelocityYaw(float *value);
}

namespace app::imu
{
    void broadcast()
    {
        //Changed original into namespaces but does not exist yet
        float x_lin_accel, y_lin_accel, z_lin_accel;
        float roll_ang_vel, pitch_ang_vel, yaw_ang_vel;
        LOG_IF_ERR(io::imu::getLinearAccelerationX(&x_lin_accel));
        LOG_IF_ERR(io::imu::getLinearAccelerationY(&y_lin_accel));
        LOG_IF_ERR(io::imu::getLinearAccelerationZ(&z_lin_accel));
        LOG_IF_ERR(io::imu::getAngularVelocityRoll(&roll_ang_vel));
        LOG_IF_ERR(io::imu::getAngularVelocityPitch(&pitch_ang_vel));
        LOG_IF_ERR(io::imu::getAngularVelocityYaw(&yaw_ang_vel));
        app_canTx_RSM_LinearAccelerationInX_set(x_lin_accel);
        app_canTx_RSM_LinearAccelerationInY_set(y_lin_accel);
        app_canTx_RSM_LinearAccelerationInZ_set(z_lin_accel);
        app_canTx_RSM_RollAngularAcceleration_set(roll_ang_vel);
        app_canTx_RSM_PitchAngularAcceleration_set(pitch_ang_vel);
        app_canTx_RSM_YawAngularAcceleration_set(yaw_ang_vel);
    }
}
