#include "app_imu.hpp"
#include "io_imus.hpp"
#include "util_errorCodes.hpp"
#include "io_imu.hpp"

extern "C"
{
    #include "app_canTx.h"
}


//i feel its doing too much lol
// static RSMImuData imu_outputs = {   .x_lin_accel = 0.0f,
//                                     .y_lin_accel = 0.0f,
//                                     .z_lin_accel = 0.0f,
//                                     .roll_ang_vel = 0.0f,
//                                     .pitch_ang_vel = 0.0f,
//                                     .yaw_ang_vel = 0.0f };

static RSMImuFaults imu_faults = {  .x_lin_accel_fault = EXIT_CODE_ERROR,
                                    .y_lin_accel_fault = EXIT_CODE_ERROR,
                                    .z_lin_accel_fault = EXIT_CODE_ERROR,
                                    .roll_ang_vel_fault = EXIT_CODE_ERROR,
                                    .pitch_ang_vel_fault = EXIT_CODE_ERROR,
                                    .yaw_ang_vel_fault = EXIT_CODE_ERROR };

//fix exitcodes not working (cmake?)

namespace app::imu
{
    void broadcast()
    {
        float x_lin_accel, y_lin_accel, z_lin_accel;
        float roll_ang_vel, pitch_ang_vel, yaw_ang_vel;
//change back to namespaces

        imu_faults.x_lin_accel_fault = io_imu_getLinearAccelerationX(&x_lin_accel);
        imu_faults.y_lin_accel_fault = io_imu_getLinearAccelerationY(&y_lin_accel);
        imu_faults.z_lin_accel_fault = io_imu_getLinearAccelerationZ(&z_lin_accel);
        imu_faults.roll_ang_vel_fault = io_imu_getAngularVelocityRoll(&roll_ang_vel);
        imu_faults.pitch_ang_vel_fault = io_imu_getAngularVelocityPitch(&pitch_ang_vel);
        imu_faults.yaw_ang_vel_fault = io_imu_getAngularVelocityYaw(&yaw_ang_vel);

        x_lin_accel =  imu_faults.x_lin_accel_fault ? x_lin_accel : 0.0f;
        y_lin_accel =  imu_faults.y_lin_accel_fault ? y_lin_accel : 0.0f;
        z_lin_accel =  imu_faults.z_lin_accel_fault ? z_lin_accel : 0.0f;
        roll_ang_vel = imu_faults.roll_ang_vel_fault ? roll_ang_vel : 0.0f;
        pitch_ang_vel = imu_faults.pitch_ang_vel_fault ? pitch_ang_vel : 0.0f;
        yaw_ang_vel = imu_faults.yaw_ang_vel_fault ? yaw_ang_vel : 0.0f;

        //if I changed it back to old notation, but I have to include this in test
        // LOG_IF_ERR(io_imu_getLinearAccelerationX(&x_lin_accel));
        // LOG_IF_ERR(io_imu_getLinearAccelerationY(&y_lin_accel));
        // LOG_IF_ERR(io_imu_getLinearAccelerationZ(&z_lin_accel));
        // LOG_IF_ERR(io_imu_getAngularVelocityRoll(&roll_ang_vel));
        // LOG_IF_ERR(io_imu_getAngularVelocityPitch(&pitch_ang_vel));
        // LOG_IF_ERR(io_imu_getAngularVelocityYaw(&yaw_ang_vel));
        app_canTx_RSM_LinearAccelerationInX_set(x_lin_accel);
        app_canTx_RSM_LinearAccelerationInY_set(y_lin_accel);
        app_canTx_RSM_LinearAccelerationInZ_set(z_lin_accel);
        app_canTx_RSM_RollAngularAcceleration_set(roll_ang_vel);
        app_canTx_RSM_PitchAngularAcceleration_set(pitch_ang_vel);
        app_canTx_RSM_YawAngularAcceleration_set(yaw_ang_vel);
    }
}
