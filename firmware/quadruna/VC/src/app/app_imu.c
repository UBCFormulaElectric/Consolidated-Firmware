#include <app_imu.h>
#include <io_imu.h>
#include <app_canTx.h>

void app_collect_imu_data(imu_data * imu)
{
    // TODO: move this along with IO files to shared if we have a rear and front IMU
    io_imu_getLinearAccelerationY(&imu->lat_accel);
    io_imu_getLinearAccelerationX(&imu->long_accel);

    app_canTx_VC_ImuAccelerationY_set(imu->lat_accel);
    app_canTx_VC_ImuAccelerationX_set(imu->long_accel);

}
