#include <assert.h>
#include "app_sbgEllipse.h"
#include "app_canTx.h"
#include "io_sbgEllipse.h"
#include "app_units.h"
#include "app_vehicleDynamicsConstants.h"

static VelocityData app_sbgEllipse_calculateVelocity(void);
static void velocityRelativeToAbsolute(VelocityData *velocity);

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

    bool is_velocity_invalid = app_canTx_VC_Warning_VelocityDataInvalid_get();

    float ekf_vel_N = 0;
    float ekf_vel_E = 0;
    float ekf_vel_D = 0;

    if (is_velocity_invalid) {
        // Calculation based Velocity
        VelocityData velocity = app_sbgEllipse_calculateVelocity();

        ekf_vel_N = velocity.north;
        ekf_vel_E = velocity.east;
        ekf_vel_D = velocity.down;
    } else {
        // EKF
        ekf_vel_N = io_sbgEllipse_getEkfNavVelocityData()->north;
        ekf_vel_E = io_sbgEllipse_getEkfNavVelocityData()->east;
        ekf_vel_D = io_sbgEllipse_getEkfNavVelocityData()->down;
    }

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

static VelocityData app_sbgEllipse_calculateVelocity()
{
    // These velocity calculations are not going to be super accurate because it
    // currently does not compute a proper relative y-axis velocity because no yaw rate 

    float wheelRadius = 0.23f; // 18 inch -> 0.46 meters (diameter) -> 0.23 meters (radius)

    float leftMotorRPM = (float)app_canTx_VC_LeftInverterSpeedCommand_get();
    float rightMotorRPM = (float)app_canTx_VC_RightInverterSpeedCommand_get();

    float leftWheelVelocity = wheelRadius * (leftMotorRPM  * M_PI_F) / (30 * GEAR_RATIO);
    float rightWheelVelocity = wheelRadius * (rightMotorRPM * M_PI_F) / (30 * GEAR_RATIO);

    VelocityData velocity;

    // This is technically velocity in the x-axis as it is relative
    velocity.north = (leftWheelVelocity + rightWheelVelocity) / 2.0f;

    // This is technically velocity in the y-axis as it is relative
    velocity.east = 0;

    velocity.down = 0;

    app_sbgEllipse_velocityRelativeToAbsolute2D(&velocity);

    return velocity;
}

static void app_sbgEllipse_velocityRelativeToAbsolute2D(VelocityData *velocity) 
{
    // very simple calculation taking the components of the x-axis velocity
    float yawAngle = io_sbgEllipse_getEulerAngles()->yaw;
    float velocityX = velocity->north;

    velocity->north = velocityX * cosf(yawAngle);
    velocity->east = velocityX * sinf(yawAngle);
}