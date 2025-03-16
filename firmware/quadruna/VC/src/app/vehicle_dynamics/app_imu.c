#include "app_imu.h"


static imuData imu_outputs = {
                                .lat_accel = 0.0,
                                .long_accel = 0.0,
                                .z_accel = 0.0,
                                .yaw_rate = 0.0,
                                .pitch_rate = 0.0,
                                .roll_rate = 0.0
                            }; 

void app_collect_imu_data()
{
    bool has_lin_accel_x = io_imu_getLinearAccelerationX(&imu_outputs.long_accel);
    bool has_lin_accel_y = io_imu_getLinearAccelerationY(&imu_outputs.lat_accel);
    bool has_lin_accel_z = io_imu_getLinearAccelerationZ(&imu_outputs.z_accel);
    bool has_ang_vel_roll  = io_imu_getAngularVelocityRoll(&imu_outputs.roll_rate);
    bool has_ang_vel_pitch = io_imu_getAngularVelocityPitch(&imu_outputs.pitch_rate);
    bool has_ang_vel_yaw   = io_imu_getAngularVelocityYaw(&imu_outputs.roll_rate);

    if (has_lin_accel_x && has_lin_accel_y && has_lin_accel_z)
    {
        app_canTx_VC_ImuAccelerationX_set(imu_outputs.lat_accel);
        app_canTx_VC_ImuAccelerationY_set(imu_outputs.long_accel);
        app_canTx_VC_ImuAccelerationZ_set(imu_outputs.z_accel);
    }

    if (has_ang_vel_roll && has_ang_vel_pitch && has_ang_vel_yaw)
    {
        app_canTx_VC_ImuAngularVelocityRoll_set(imu_outputs.roll_rate);
        app_canTx_VC_ImuAngularVelocityPitch_set(imu_outputs.pitch_rate);
        app_canTx_VC_ImuAngularVelocityYaw_set(imu_outputs.yaw_rate);
    }

}


imuData app_get_imu_data()
{
    return imu_outputs; 
}