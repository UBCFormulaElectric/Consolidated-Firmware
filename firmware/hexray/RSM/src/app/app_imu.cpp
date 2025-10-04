#include "app_imu.hpp"
#include "io_imu.hpp"
// #include "app_canTx.hpp" //Not created yet

void app_imu_broadcast()
{
    float x_lin_accel, y_lin_accel, z_lin_accel;
    float roll_ang_vel, pitch_ang_vel, yaw_ang_vel;
    LOG_IF_ERR(io_imu_getLinearAccelerationX(&x_lin_accel));
    LOG_IF_ERR(io_imu_getLinearAccelerationY(&y_lin_accel));
    LOG_IF_ERR(io_imu_getLinearAccelerationZ(&z_lin_accel));
    LOG_IF_ERR(io_imu_getAngularVelocityRoll(&roll_ang_vel));
    LOG_IF_ERR(io_imu_getAngularVelocityPitch(&pitch_ang_vel));
    LOG_IF_ERR(io_imu_getAngularVelocityYaw(&yaw_ang_vel));
    app_canTx_RSM_LinearAccelerationInX_set(x_lin_accel);
    app_canTx_RSM_LinearAccelerationInY_set(y_lin_accel);
    app_canTx_RSM_LinearAccelerationInZ_set(z_lin_accel);
    app_canTx_RSM_RollAngularAcceleration_set(roll_ang_vel);
    app_canTx_RSM_PitchAngularAcceleration_set(pitch_ang_vel);
    app_canTx_RSM_YawAngularAcceleration_set(yaw_ang_vel);
}

//QUESTION: What is LOG_IF_ERR 