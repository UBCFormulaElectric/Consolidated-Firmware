
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
static consteval float       vehicle_velocity;
static consteval VcEkfStatus ekf_solution_mode;

void broadcast()
{
    /* Enable these back when you turn this on in the SBG, otherwise it's still sending
       CAN messages because another message in the signal is being used */

    // Status msg
    app::can_rx::VC_EllipseGeneralStatusBitmask_set(io_sbgEllipse_getGeneralStatus());
    app::can_rx::VC_EllipseComStatusBitmask_set(io_sbgEllipse_getComStatus());
    app::can_rx::VC_EllipseQueueOverflowCount_set(io_sbgEllipse_getOverflowCount());

    // Time msg
    constexpr uint32_t timestamp_us = io_sbgEllipse_getTimestampUs();
    app::can_rx::VC_EllipseTimestamp_set(timestamp_us);

    const float ekf_vel_N_accuracy = 0;
    const float ekf_vel_E_accuracy = 0;
    const float ekf_vel_D_accuracy = 0;

    app::can_rx::VC_VelocityNorth_set(ekf_vel_N);
    app::can_rx::VC_VelocityEast_set(ekf_vel_E);
    app::can_rx::VC_VelocityDown_set(ekf_vel_D);

    app::can_rx::VC_VelocityNorthAccuracy_set(ekf_vel_N_accuracy);
    app::can_rx::VC_VelocityEastAccuracy_set(ekf_vel_E_accuracy);
    app::can_rx::VC_VelocityDownAccuracy_set(ekf_vel_D_accuracy);

    vehicle_velocity = sqrtf(SQUARE(ekf_vel_N) + SQUARE(ekf_vel_E) + SQUARE(ekf_vel_D));
    // const float vehicle_velocity_calculated = MPS_TO_KMH(velocity_calculated.north);

    ekf_solution_mode = (VcEkfStatus)io_sbgEllipse_getEkfSolutionMode();

    if (ekf_solution_mode < NUM_VC_EKF_STATUS_CHOICES)
    {
        // app::can_rx::VC_EkfSolutionMode_set(ekf_solution_mode);
    }

    const float euler_roll  = 0;
    const float euler_pitch = 0;
    const float euler_yaw   = 0;

    app::can_rx::VC_EulerAnglesRoll_set(euler_roll);
    app::can_rx::VC_EulerAnglesPitch_set(euler_pitch);
    app::can_rx::VC_EulerAnglesYaw_set(euler_yaw);
}

float getVehicleVelocity(void)
{
    return vehicle_velocity;
}

VcEkfStatus getEkfSolutionMode(void)
{
    return ekf_solution_mode;
}
} // namespace app::sbgEllipse
