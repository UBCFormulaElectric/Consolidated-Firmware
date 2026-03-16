
#include "app_sbgEllipse.hpp"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_utils.h"
#include "hw_assert.hpp"

extern "C"
{
#include "app_units.h"
#include "app_vehicleDynamicsConstants.h"
}

namespace app::sbgEllipse
{
static float                       vehicle_velocity;
static app::can_utils::VcEkfStatus ekf_solution_mode;
static constexpr int NUM_VC_EKF_STATUS_CHOICES{ 5 };

void broadcast()
{
    /* Enable these back when you turn this on in the SBG, otherwise it's still sending
       CAN messages because another message in the signal is being used */

    // Status msg
    app::can_tx::VC_EllipseGeneralStatusBitmask_set(io_sbgEllipse_getGeneralStatus());
    app::can_tx::VC_EllipseComStatusBitmask_set(io_sbgEllipse_getComStatus());
    app::can_tx::VC_EllipseQueueOverflowCount_set(io_sbgEllipse_getOverflowCount());

    // Time msg
    constexpr uint32_t timestamp_us = io_sbgEllipse_getTimestampUs();
    app::can_tx::VC_EllipseTimestamp_set(timestamp_us);

    const float ekf_vel_N_accuracy = 0.0f;
    const float ekf_vel_E_accuracy = 0.0f;
    const float ekf_vel_D_accuracy = 0.0f;

    app::can_tx::VC_VelocityNorth_set(ekf_vel_N);
    app::can_tx::VC_VelocityEast_set(ekf_vel_E);
    app::can_tx::VC_VelocityDown_set(ekf_vel_D);

    app::can_tx::VC_VelocityNorthAccuracy_set(ekf_vel_N_accuracy);
    app::can_tx::VC_VelocityEastAccuracy_set(ekf_vel_E_accuracy);
    app::can_tx::VC_VelocityDownAccuracy_set(ekf_vel_D_accuracy);

    vehicle_velocity = sqrtf(SQUARE(ekf_vel_N) + SQUARE(ekf_vel_E) + SQUARE(ekf_vel_D));
    // const float vehicle_velocity_calculated = MPS_TO_KMH(velocity_calculated.north);

    ekf_solution_mode = (app::can_utils::VcEkfStatus)io_sbgEllipse_getEkfSolutionMode();

    if (static_cast<int>(ekf_solution_mode) < NUM_VC_EKF_STATUS_CHOICES)
    {
        app::can_tx::VC_EkfSolutionMode_set(ekf_solution_mode);
    }

    const float euler_roll  = 0.0f;
    const float euler_pitch = 0.0f;
    const float euler_yaw   = 0.0f;

    app::can_tx::VC_EulerAnglesRoll_set(euler_roll);
    app::can_tx::VC_EulerAnglesPitch_set(euler_pitch);
    app::can_tx::VC_EulerAnglesYaw_set(euler_yaw);
}
io::sbgEllipse::VelocityData calculateVelocity(io::sbgEllipse::Vector3 *position){
    // These velocity calculations are not going to be super accurate because it
    // currently does not compute a proper relative y-axis velocity because no yaw rate

    const float rightMotorRPM = (float)-app_canRx_INVR_MotorSpeed_get();
    const float leftMotorRPM  = (float)app_canRx_INVL_MotorSpeed_get();

    float leftWheelVelocity  = MOTOR_RPM_TO_KMH(leftMotorRPM);
    float rightWheelVelocity = MOTOR_RPM_TO_KMH(rightMotorRPM);

    float velocityX = (leftWheelVelocity + rightWheelVelocity) / 2.0f;

    // This is technically velocity in the x-axis as it is relative
    velocity.north = velocityX;

    // This is technically velocity in the y-axis as it is relative
    velocity->east = 0;

    velocity->down = 0;
}

float getVehicleVelocity(void)
{
    return vehicle_velocity;
}

app::can_utils::VcEkfStatus getEkfSolutionMode(void)
{
    return ekf_solution_mode;
}
} // namespace app::sbgEllipse
