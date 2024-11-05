#include <assert.h>
#include "app_sbgEllipse.h"
#include "app_canTx.h"
#include "io_sbgEllipse.h"

void app_sbgEllipse_broadcast()
{
    // Status msg
    const uint16_t general_status = io_sbgEllipse_getGeneralStatus();
    const uint32_t com_status     = io_sbgEllipse_getComStatus();

    app_canTx_VC_EllipseGeneralStatusBitmask_set(general_status);
    app_canTx_VC_EllipseComStatusBitmask_set(com_status);

    // Time msg
    const uint32_t timestamp_us = io_sbgEllipse_getTimestampUs();
    app_canTx_VC_EllipseTimestamp_set(timestamp_us);

    // EKF
    const float ekf_vel_N = io_sbgEllipse_getEkfNavVelocityData()->north;
    const float ekf_vel_E = io_sbgEllipse_getEkfNavVelocityData()->east;
    const float ekf_vel_D = io_sbgEllipse_getEkfNavVelocityData()->down;

    app_canTx_VC_VelocityNorth_set(ekf_vel_N);
    app_canTx_VC_VelocityEast_set(ekf_vel_E);
    app_canTx_VC_VelocityDown_set(ekf_vel_D);

    // EKF
    const double ekf_pos_lat  = io_sbgEllipse_getEkfNavPositionData()->latitude;
    const double ekf_pos_long = io_sbgEllipse_getEkfNavPositionData()->longitude;

    app_canTx_VC_Latitude_set((float)ekf_pos_lat);
    app_canTx_VC_Longtitude_set((float)ekf_pos_long);

    // Acceleration msg
    // const float forward_accel  = io_sbgEllipse_getImuAccelerations()->x;
    // const float lateral_accel  = io_sbgEllipse_getImuAccelerations()->y;
    // const float vertical_accel = io_sbgEllipse_getImuAccelerations()->z;

    // app_canTx_VC_AccelerationForward_set(forward_accel);
    // app_canTx_VC_AccelerationLateral_set(lateral_accel);
    // app_canTx_VC_AccelerationVertical_set(vertical_accel);

    // Angular velocity msg
    // const float ang_vel_roll  = io_sbgEllipse_getImuAngularVelocities()->roll;
    // const float ang_vel_pitch = io_sbgEllipse_getImuAngularVelocities()->pitch;
    // const float ang_vel_yaw   = io_sbgEllipse_getImuAngularVelocities()->yaw;

    // app_canTx_VC_AngularVelocityRoll_set((int)ang_vel_roll);
    // app_canTx_VC_AngularVelocityPitch_set((int)ang_vel_pitch);
    // app_canTx_VC_AngularVelocityYaw_set((int)ang_vel_yaw);

    // Euler angles msg
    const float euler_roll  = io_sbgEllipse_getEulerAngles()->roll;
    const float euler_pitch = io_sbgEllipse_getEulerAngles()->pitch;
    const float euler_yaw   = io_sbgEllipse_getEulerAngles()->yaw;

    app_canTx_VC_EulerAnglesRoll_set(euler_roll);
    app_canTx_VC_EulerAnglesPitch_set(euler_pitch);
    app_canTx_VC_EulerAnglesYaw_set(euler_yaw);
}
