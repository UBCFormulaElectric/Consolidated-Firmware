#include <assert.h>
#include "app_sbgEllipse.h"
#include "App_CanTx.h"
#include "io_sbgEllipse.h"

void app_sbgEllipse_broadcast()
{
    // Status msg
    const uint16_t general_status = io_sbgEllipse_getGeneralStatus();
    const uint32_t com_status     = io_sbgEllipse_getComStatus();

    App_CanTx_DCM_EllipseGeneralStatusBitmask_Set(general_status);
    App_CanTx_DCM_EllipseComStatusBitmask_Set(com_status);

    // Time msg
    const uint32_t timestamp_us = io_sbgEllipse_getTimestampUs();
    App_CanTx_DCM_EllipseTimestamp_Set(timestamp_us);

    // Acceleration msg
    const float forward_accel  = io_sbgEllipse_getSensorOutput(SBG_ELLIPSE_OUT_ACCELERATION_X);
    const float lateral_accel  = io_sbgEllipse_getSensorOutput(SBG_ELLIPSE_OUT_ACCELERATION_Y);
    const float vertical_accel = io_sbgEllipse_getSensorOutput(SBG_ELLIPSE_OUT_ACCELERATION_Z);

    App_CanTx_DCM_AccelerationForward_Set(forward_accel);
    App_CanTx_DCM_AccelerationLateral_Set(lateral_accel);
    App_CanTx_DCM_AccelerationVertical_Set(vertical_accel);

    // Angular velocity msg
    const float ang_vel_roll  = io_sbgEllipse_getSensorOutput(SBG_ELLIPSE_OUT_ANGULAR_VELOCITY_ROLL);
    const float ang_vel_pitch = io_sbgEllipse_getSensorOutput(SBG_ELLIPSE_OUT_ANGULAR_VELOCITY_PITCH);
    const float ang_vel_yaw   = io_sbgEllipse_getSensorOutput(SBG_ELLIPSE_OUT_ANGULAR_VELOCITY_YAW);

    App_CanTx_DCM_AngularVelocityRoll_Set((int)ang_vel_roll);
    App_CanTx_DCM_AngularVelocityPitch_Set((int)ang_vel_pitch);
    App_CanTx_DCM_AngularVelocityYaw_Set((int)ang_vel_yaw);

    // Euler angles msg
    const float euler_roll  = io_sbgEllipse_getSensorOutput(SBG_ELLIPSE_OUT_EULER_ROLL);
    const float euler_pitch = io_sbgEllipse_getSensorOutput(SBG_ELLIPSE_OUT_EULER_PITCH);
    const float euler_yaw   = io_sbgEllipse_getSensorOutput(SBG_ELLIPSE_OUT_EULER_YAW);

    App_CanTx_DCM_EulerAnglesRoll_Set(euler_roll);
    App_CanTx_DCM_EulerAnglesPitch_Set(euler_pitch);
    App_CanTx_DCM_EulerAnglesYaw_Set(euler_yaw);
}
