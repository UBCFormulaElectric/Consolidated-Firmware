#include <app_imu.h>
#include <io_imu.h>
#include <app_canTx.h>

void app_collectImuData(imuData *imu)
{
    io_imu_getLinearAccelerationX(&imu->long_accel);
    io_imu_getLinearAccelerationY(&imu->lat_accel);
    io_imu_getAngularVelocityYaw(&imu->yaw_rate);

    app_canTx_VC_ImuAccelerationX_set(imu->long_accel);
    app_canTx_VC_ImuAccelerationY_set(imu->lat_accel);
    app_canTx_VC_ImuAngularVelocityYaw_set(imu->yaw_rate);
}