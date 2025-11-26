#include "app_imu.h"
#include "app_canAlerts.h"

static ImuData imu_outputs = { .long_accel = 0.0f,
                               .lat_accel  = 0.0f,
                               .z_accel    = 0.0f,
                               .roll_rate  = 0.0f,
                               .pitch_rate = 0.0f,
                               .yaw_rate   = 0.0f };

static ImuFaults imu_faults = { .long_accel_fault = EXIT_CODE_ERROR,
                                .lat_accel_fault  = EXIT_CODE_ERROR,
                                .z_accel_fault    = EXIT_CODE_ERROR,
                                .roll_rate_fault  = EXIT_CODE_ERROR,
                                .pitch_rate_fault = EXIT_CODE_ERROR,
                                .yaw_rate_fault   = EXIT_CODE_ERROR };

void app_imu_init()
{
    app_canAlerts_VC_Info_ImuFault_set(IS_EXIT_OK(io_imu_init()));
}

void app_imu_broadcast()
{
    imu_faults.long_accel_fault = io_imu_getLinearAccelerationX(&imu_outputs.long_accel);
    imu_faults.lat_accel_fault  = io_imu_getLinearAccelerationY(&imu_outputs.lat_accel);
    imu_faults.z_accel_fault    = io_imu_getLinearAccelerationZ(&imu_outputs.z_accel);
    imu_faults.roll_rate_fault  = io_imu_getAngularVelocityRoll(&imu_outputs.roll_rate);
    imu_faults.pitch_rate_fault = io_imu_getAngularVelocityPitch(&imu_outputs.pitch_rate);
    imu_faults.yaw_rate_fault   = io_imu_getAngularVelocityYaw(&imu_outputs.yaw_rate);

    // Set outputs to 0 if their respective transactions fault
    imu_outputs.long_accel = imu_faults.long_accel_fault ? 0.0f : imu_outputs.long_accel;
    imu_outputs.lat_accel  = imu_faults.lat_accel_fault ? 0.0f : imu_outputs.lat_accel;
    imu_outputs.z_accel    = imu_faults.z_accel_fault ? 0.0f : imu_outputs.z_accel;
    imu_outputs.roll_rate  = imu_faults.roll_rate_fault ? 0.0f : imu_outputs.roll_rate;
    imu_outputs.pitch_rate = imu_faults.pitch_rate_fault ? 0.0f : imu_outputs.pitch_rate;
    imu_outputs.yaw_rate   = imu_faults.yaw_rate_fault ? 0.0f : imu_outputs.yaw_rate;

    app_canTx_VC_ImuAccelerationX_set(imu_outputs.lat_accel);
    app_canTx_VC_ImuAccelerationY_set(imu_outputs.long_accel);
    app_canTx_VC_ImuAccelerationZ_set(imu_outputs.z_accel);
    app_canTx_VC_ImuAngularVelocityRoll_set(imu_outputs.roll_rate);
    app_canTx_VC_ImuAngularVelocityPitch_set(imu_outputs.pitch_rate);
    app_canTx_VC_ImuAngularVelocityYaw_set(imu_outputs.yaw_rate);
}

const ImuData *app_imu_getData()
{
    return &imu_outputs;
}

const ImuFaults *app_imu_getFaultData()
{
    return &imu_faults;
}