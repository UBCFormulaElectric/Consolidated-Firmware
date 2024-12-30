#include <assert.h>
#include "app_sbgEllipse.h"
#include "app_canTx.h"
#include "app_utils.h"
#include "app_units.h"
#include "io_sbgEllipse.h"

float vehicle_velocity = 0.0f;

void app_sbgEllipse_broadcast()
{
    /* Enable these back when you turn this on in the SBG, otherwise it's still sending
       CAN messages because another message in the signal is being used */

    // Status msg
    // const uint16_t general_status = io_sbgEllipse_getGeneralStatus();
    // const uint32_t com_status     = io_sbgEllipse_getComStatus();

    // app_canTx_VC_EllipseGeneralStatusBitmask_set(general_status);
    // app_canTx_VC_EllipseComStatusBitmask_set(com_status);

    // Time msg
    // const uint32_t timestamp_us = io_sbgEllipse_getTimestampUs();
    // app_canTx_VC_EllipseTimestamp_set(timestamp_us);

    // Velocity EKF
    const float ekf_vel_N = io_sbgEllipse_getEkfNavVelocityData()->north;
    const float ekf_vel_E = io_sbgEllipse_getEkfNavVelocityData()->east;
    const float ekf_vel_D = io_sbgEllipse_getEkfNavVelocityData()->down;

    const float ekf_vel_N_accuracy = io_sbgEllipse_getEkfNavVelocityData()->north_std_dev;
    const float ekf_vel_E_accuracy = io_sbgEllipse_getEkfNavVelocityData()->east_std_dev;
    const float ekf_vel_D_accuracy = io_sbgEllipse_getEkfNavVelocityData()->down_std_dev;

    app_canTx_VC_VelocityNorth_set(ekf_vel_N);
    app_canTx_VC_VelocityEast_set(ekf_vel_E);
    app_canTx_VC_VelocityDown_set(ekf_vel_D);

    app_canTx_VC_VelocityNorthAccuracy_set(ekf_vel_N_accuracy);
    app_canTx_VC_VelocityEastAccuracy_set(ekf_vel_E_accuracy);
    app_canTx_VC_VelocityDownAccuracy_set(ekf_vel_D_accuracy);

    vehicle_velocity = sqrtf(SQUARE(ekf_vel_N) + SQUARE(ekf_vel_E) + SQUARE(ekf_vel_D));

    app_canTx_VC_VehicleVelocity_set(MPS_TO_KMH(vehicle_velocity));

    // Position EKF
    // const double ekf_pos_lat  = io_sbgEllipse_getEkfNavPositionData()->latitude;
    // const double ekf_pos_long = io_sbgEllipse_getEkfNavPositionData()->longitude;

    // app_canTx_VC_Latitude_set((float)ekf_pos_lat);
    // app_canTx_VC_Longtitude_set((float)ekf_pos_long);

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
    const float euler_roll  = io_sbgEllipse_getEkfEulerAngles()->roll;
    const float euler_pitch = io_sbgEllipse_getEkfEulerAngles()->pitch;
    const float euler_yaw   = io_sbgEllipse_getEkfEulerAngles()->yaw;

    app_canTx_VC_EulerAnglesRoll_set(euler_roll);
    app_canTx_VC_EulerAnglesPitch_set(euler_pitch);
    app_canTx_VC_EulerAnglesYaw_set(euler_yaw);
}
