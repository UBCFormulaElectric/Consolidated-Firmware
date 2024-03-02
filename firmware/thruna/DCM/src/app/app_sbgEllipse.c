#include <assert.h>
#include "app_sbgEllipse.h"
#include "app_canTx.h"
#include "io_sbgEllipse.h"

void app_sbgEllipse_broadcast()
{
    // Status msg
    const uint16_t general_status = io_sbgEllipse_getGeneralStatus();
    const uint32_t com_status     = io_sbgEllipse_getComStatus();

    // Overflow msg
    const uint32_t overflow_status = io_sbgEllipse_getOverflowCount();

    app_canTx_DCM_EllipseGeneralStatusBitmask_set(general_status);
    app_canTx_DCM_EllipseComStatusBitmask_set(com_status);
    app_canTx_DCM_Warning_SbgRxOverflowCount_set(overflow_status);

    // Time msg
    const uint32_t timestamp_us = io_sbgEllipse_getTimestampUs();
    app_canTx_DCM_EllipseTimestamp_set(timestamp_us);

    // Acceleration msg
    const float forward_accel  = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_ACCELERATION_X);
    const float lateral_accel  = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_ACCELERATION_Y);
    const float vertical_accel = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_ACCELERATION_Z);

    app_canTx_DCM_AccelerationForward_set(forward_accel);
    app_canTx_DCM_AccelerationLateral_set(lateral_accel);
    app_canTx_DCM_AccelerationVertical_set(vertical_accel);

    // Angular velocity msg
    const float ang_vel_roll  = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_ANGULAR_VELOCITY_ROLL);
    const float ang_vel_pitch = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_ANGULAR_VELOCITY_PITCH);
    const float ang_vel_yaw   = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_ANGULAR_VELOCITY_YAW);

    app_canTx_DCM_AngularVelocityRoll_set((int)ang_vel_roll);
    app_canTx_DCM_AngularVelocityPitch_set((int)ang_vel_pitch);
    app_canTx_DCM_AngularVelocityYaw_set((int)ang_vel_yaw);

    // Euler angles msg
    const float euler_roll  = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_EULER_ROLL);
    const float euler_pitch = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_EULER_PITCH);
    const float euler_yaw   = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_EULER_YAW);

    app_canTx_DCM_EulerAnglesRoll_set(euler_roll);
    app_canTx_DCM_EulerAnglesPitch_set(euler_pitch);
    app_canTx_DCM_EulerAnglesYaw_set(euler_yaw);

    // Gps position msg
    const float position_status = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_GPS_POS_STATUS);
    const float latitude        = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_GPS_LAT);
    const float latitude_acc    = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_GPS_LAT_ACC);
    const float longitude       = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_GPS_LONG);
    const float longitude_acc   = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_GPS_LONG_ACC);
    const float altitude        = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_GPS_ALT);
    const float altitude_acc    = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_GPS_ALT_ACC);

    app_canTx_DCM_PositionStatus_set(position_status);
    app_canTx_DCM_Latitude_set(latitude);
    app_canTx_DCM_LatitudeAccuracy_set(latitude_acc);
    app_canTx_DCM_Longtitude_set(longitude);
    app_canTx_DCM_LongtitudeAccuracy_set(longitude_acc);
    app_canTx_DCM_Altitude_set(altitude);
    app_canTx_DCM_AltitudeAccuracy_set(altitude_acc);

    // Gps velocity msg
    const float velocity_status    = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_GPS_VEL_STATUS);
    const float velocity_north     = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_GPS_VEL_N);
    const float velocity_north_acc = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_GPS_VEL_N_ACC);
    const float velocity_east      = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_GPS_VEL_E);
    const float velocity_east_acc  = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_GPS_VEL_E_ACC);
    const float velocity_down      = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_GPS_VEL_D);
    const float velocity_down_acc  = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_GPS_VEL_D_ACC);

    app_canTx_DCM_VelocityStatus_set(velocity_status);
    app_canTx_DCM_VelocityNorth_set(velocity_north);
    app_canTx_DCM_VelocityNorthAccuracy_set(velocity_north_acc);
    app_canTx_DCM_VelocityEast_set(velocity_east);
    app_canTx_DCM_VelocityEastAccuracy_set(velocity_east_acc);
    app_canTx_DCM_VelocityDown_set(velocity_down);
    app_canTx_DCM_VelocityDownAccuracy_set(velocity_down_acc);
}
