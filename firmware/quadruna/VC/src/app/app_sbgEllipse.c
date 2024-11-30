#include <assert.h>
#include "app_sbgEllipse.h"
#include "app_canTx.h"
#include "app_utils.h"
#include "app_units.h"
#include "io_sbgEllipse.h"
#include "app_vehicleDynamicsConstants.h"

static VelocityData app_sbgEllipse_calculateVelocity(void);
static void app_sbgEllipse_velocityRelativeToAbsolute2D(VelocityData *velocity);

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

    VcEkfStatus sbgSolutionMode = app_canTx_VC_EkfSolutionMode_get();

    float ekf_vel_N = 0;
    float ekf_vel_E = 0;
    float ekf_vel_D = 0;

    if (sbgSolutionMode != POSITION) {
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

    const float ekf_vel_N_accuracy = io_sbgEllipse_getEkfNavVelocityData()->north_std_dev;
    const float ekf_vel_E_accuracy = io_sbgEllipse_getEkfNavVelocityData()->east_std_dev;
    const float ekf_vel_D_accuracy = io_sbgEllipse_getEkfNavVelocityData()->down_std_dev;

    app_canTx_VC_VelocityNorth_set(ekf_vel_N);
    app_canTx_VC_VelocityEast_set(ekf_vel_E);
    app_canTx_VC_VelocityDown_set(ekf_vel_D);

    app_canTx_VC_VelocityNorthAccuracy_set(ekf_vel_N_accuracy);
    app_canTx_VC_VelocityEastAccuracy_set(ekf_vel_E_accuracy);
    app_canTx_VC_VelocityDownAccuracy_set(ekf_vel_D_accuracy);

    const float vehicle_velocity = sqrtf(SQUARE(ekf_vel_N) + SQUARE(ekf_vel_E) + SQUARE(ekf_vel_D));

    app_canTx_VC_VehicleVelocity_set(MPS_TO_KMH(vehicle_velocity));

    // Position EKF
    // const double ekf_pos_lat  = io_sbgEllipse_getEkfNavPositionData()->latitude;
    // const double ekf_pos_long = io_sbgEllipse_getEkfNavPositionData()->longitude;

    // app_canTx_VC_Latitude_set((float)ekf_pos_lat);
    // app_canTx_VC_Longtitude_set((float)ekf_pos_long);

    // EKF
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

static VelocityData app_sbgEllipse_calculateVelocity()
{
    // These velocity calculations are not going to be super accurate because it
    // currently does not compute a proper relative y-axis velocity because no yaw rate 

    float wheelRadius = 0.23f; // 18 inch -> 0.46 meters (diameter) -> 0.23 meters (radius)

    float leftMotorRPM = (float)app_canTx_VC_LeftInverterSpeedCommand_get();
    float rightMotorRPM = (float)app_canTx_VC_RightInverterSpeedCommand_get();

    float leftWheelVelocity = wheelRadius * (leftMotorRPM  * M_PI_F) / (30 * GEAR_RATIO);
    float rightWheelVelocity = wheelRadius * (rightMotorRPM * M_PI_F) / (30 * GEAR_RATIO);
    // float yawRate = app_canTx_VC_

    float velocityX = (leftWheelVelocity + rightWheelVelocity) / 2.0f;



    VelocityData velocity;

    // This is technically velocity in the x-axis as it is relative
    velocity.north = 

    // This is technically velocity in the y-axis as it is relative
    velocity.east = 0;

    velocity.down = 0;

    app_sbgEllipse_velocityRelativeToAbsolute2D(&velocity);

    return velocity;
}

static void app_sbgEllipse_velocityRelativeToAbsolute2D(VelocityData *velocity) 
{
    // very simple calculation taking the components of the x-axis velocity
    float yawAngle = io_sbgEllipse_getEkfEulerAngles()->yaw;
    float velocityX = velocity->north;

    velocity->north = velocityX * cosf(yawAngle);
    velocity->east = velocityX * sinf(yawAngle);
}