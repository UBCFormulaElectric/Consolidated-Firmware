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

    // Acceleration msg
    const float forward_accel  = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_ACCELERATION_X);
    const float lateral_accel  = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_ACCELERATION_Y);
    const float vertical_accel = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_ACCELERATION_Z);

    app_canTx_VC_AccelerationForward_set(forward_accel);
    app_canTx_VC_AccelerationLateral_set(lateral_accel);
    app_canTx_VC_AccelerationVertical_set(vertical_accel);

    // Angular velocity msg
    const float ang_vel_roll  = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_ANGULAR_VELOCITY_ROLL);
    const float ang_vel_pitch = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_ANGULAR_VELOCITY_PITCH);
    const float ang_vel_yaw   = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_ANGULAR_VELOCITY_YAW);

    app_canTx_VC_AngularVelocityRoll_set((int)ang_vel_roll);
    app_canTx_VC_AngularVelocityPitch_set((int)ang_vel_pitch);
    app_canTx_VC_AngularVelocityYaw_set((int)ang_vel_yaw);

    // Euler angles msg
    const float euler_roll  = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_EULER_ROLL);
    const float euler_pitch = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_EULER_PITCH);
    const float euler_yaw   = io_sbgEllipse_getSensorOutput(ELLIPSE_OUTPUT_EULER_YAW);

    app_canTx_VC_EulerAnglesRoll_set(euler_roll);
    app_canTx_VC_EulerAnglesPitch_set(euler_pitch);
    app_canTx_VC_EulerAnglesYaw_set(euler_yaw);
}
