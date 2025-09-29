#include "app_imu.h"

static ImuData imu_outputs = { .lat_accel  = 0.0f,
                               .long_accel = 0.0f,
                               .z_accel    = 0.0f,
                               .yaw_rate   = 0.0f,
                               .pitch_rate = 0.0f,
                               .roll_rate  = 0.0f };

void app_imu_broadcast()
{
    io_imu_getLinearAccelerationX(&imu_outputs.long_accel);
    io_imu_getLinearAccelerationY(&imu_outputs.lat_accel);
    io_imu_getLinearAccelerationZ(&imu_outputs.z_accel);
    io_imu_getAngularVelocityRoll(&imu_outputs.roll_rate);
    io_imu_getAngularVelocityPitch(&imu_outputs.pitch_rate);
    io_imu_getAngularVelocityYaw(&imu_outputs.yaw_rate);

    app_canTx_VC_ImuAccelerationX_set(imu_outputs.lat_accel);
    app_canTx_VC_ImuAccelerationY_set(imu_outputs.long_accel);
    app_canTx_VC_ImuAccelerationZ_set(imu_outputs.z_accel);

    app_canTx_VC_ImuAngularVelocityRoll_set(imu_outputs.roll_rate);
    app_canTx_VC_ImuAngularVelocityPitch_set(imu_outputs.pitch_rate);
    app_canTx_VC_ImuAngularVelocityYaw_set(imu_outputs.yaw_rate);
}

const ImuData *app_get_imu_struct()
{
    return &imu_outputs;
}