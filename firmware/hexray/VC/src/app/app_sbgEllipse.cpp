
#include "app_sbgEllipse.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_utils.hpp"
#include "hw_assert.hpp"

extern "C"
{
#include "app_units.h"
#include "app_vehicleDynamicsConstants.h"
}

namespace app::sbgEllipse
{
using namespcace                   io::sbgEllipse;
static app::can_utils::VcEkfStatus ekf_solution_mode;
static float                       vehicle_velocity;
static constexpr int               NUM_VC_EKF_STATUS_CHOICES{ 5 };

void broadcast()
{
    /* Enable these back when you turn this on in the SBG, otherwise it's still sending
       CAN messages because another message in the signal is being used */

    // Status msg
    app::can_tx::VC_EllipseGeneralStatusBitmask_set(getGeneralStatus());
    app::can_tx::VC_EllipseComStatusBitmask_set(getComStatus());
    app::can_tx::VC_EllipseQueueOverflowCount_set(getOverflowCount());

    // Time msg
    constexpr uint32_t timestamp_us = getTimestampUs();
    app::can_tx::VC_EllipseTimestamp_set(timestamp_us);

    // EKF
    const static VelocityData *VelData   = getEkfNavVelocityData();
    float                ekf_vel_N = VelData->north;
    float                ekf_vel_E = VelData->east;
    float                ekf_vel_D = VelData->down;
    vehicle_velocity               = getVehicleVelocity(&VelData);
    const float ekf_vel_N_accuracy = VelData->north_std_dev;
    const float ekf_vel_E_accuracy = VelData->east_std_dev;
    const float ekf_vel_D_accuracy = VelData->down_std_dev;

    app::can_tx::VC_VelocityNorth_set(ekf_vel_N);
    app::can_tx::VC_VelocityEast_set(ekf_vel_E);
    app::can_tx::VC_VelocityDown_set(ekf_vel_D);

    app::can_tx::VC_VelocityNorthAccuracy_set(ekf_vel_N_accuracy);
    app::can_tx::VC_VelocityEastAccuracy_set(ekf_vel_E_accuracy);
    app::can_tx::VC_VelocityDownAccuracy_set(ekf_vel_D_accuracy);

    // Velocity
    const float calculated_velocity = calculateVelocity(velData);

    ekf_solution_mode = (app::can_utils::VcEkfStatus)getEkfSolutionMode();

    if (static_cast<int>(ekf_solution_mode) < NUM_VC_EKF_STATUS_CHOICES)
    {
        app::can_tx::VC_EkfSolutionMode_set(ekf_solution_mode);
    }

    const Attitude *euler_angles = getEkfEulerAngles();
    const float     euler_roll   = euler_angles->roll;
    const float     euler_pitch  = euler_angles->pitch;
    const float     euler_yaw    = euler_angles->yaw;

    app::can_tx::VC_EulerAnglesRoll_set(euler_roll);
    app::can_tx::VC_EulerAnglesPitch_set(euler_pitch);
    app::can_tx::VC_EulerAnglesYaw_set(euler_yaw);
}

float getVehicleVelocity(VelocityData *VelData)
{
    return sqrtf(SQUARE(VelData->north) + SQUARE(VelData->east) + SQUARE(VelData->down));
}

app::can_utils::VcEkfStatus getEkfSolutionMode(void)
{
    return ekf_solution_mode;
}
} // namespace app::sbgEllipse
