#include "app_imu.hpp"

#include "io_imu.h"
//#include "app_canTx.h"    ***********Implement later***********
#include "io_log.h"

void app_imu_broadcast(void)
{
    float x_lin_accel, y_lin_accel, z_lin_accel;
    float roll_ang_vel, pitch_ang_vel, yaw_ang_vel;

    LOG_IF_ERR(io_imu_getLinearAccelerationX(&x_lin_accel));
    LOG_IF_ERR(io_imu_getLinearAccelerationY(&y_lin_accel));
    LOG_IF_ERR(io_imu_getLinearAccelerationZ(&z_lin_accel));
    LOG_IF_ERR(io_imu_getAngularVelocityRoll(&roll_ang_vel));
    LOG_IF_ERR(io_imu_getAngularVelocityPitch(&pitch_ang_vel));
    LOG_IF_ERR(io_imu_getAngularVelocityYaw(&yaw_ang_vel));

    /*app_canTx_FSM_LinearAccelerationX_set(x_lin_accel);   ***********Implement later***********
    app_canTx_FSM_LinearAccelerationY_set(y_lin_accel);
    app_canTx_FSM_LinearAccelerationZ_set(z_lin_accel);
    app_canTx_FSM_RollRate_set(roll_ang_vel);
    app_canTx_FSM_PitchRate_set(pitch_ang_vel);
    app_canTx_FSM_YawRate_set(yaw_ang_vel);*/
}