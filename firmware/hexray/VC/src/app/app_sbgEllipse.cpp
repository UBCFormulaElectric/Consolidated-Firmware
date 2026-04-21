
#include "app_sbgEllipse.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "util_utils.hpp"
#include "app_math.hpp"

namespace app::sbgEllipse
{
static app::can_utils::VcEkfStatus ekf_solution_mode;
static constexpr int               NUM_VC_EKF_STATUS_CHOICES{ 5 };

void broadcast()
{
    /* Enable these back when you turn this on in the SBG, otherwise it's still sending
       CAN messages because another message in the signal is being used */

    // Status msg
    app::can_tx::VC_EllipseGeneralStatusBitmask_set(io::sbgEllipse::getGeneralStatus());
    app::can_tx::VC_EllipseComStatusBitmask_set(io::sbgEllipse::getComStatus());
    app::can_tx::VC_EllipseQueueOverflowCount_set(io::sbgEllipse::getOverflowCount());

    // Time msg
    uint32_t timestamp_us = io::sbgEllipse::getTimestampUs();
    app::can_tx::VC_EllipseTimestamp_set(timestamp_us);

    // EKF
    const static io::sbgEllipse::VelocityData VelData            = io::sbgEllipse::getEkfNavVelocityData();
    float                                     ekf_vel_N          = VelData.north;
    float                                     ekf_vel_E          = VelData.east;
    float                                     ekf_vel_D          = VelData.down;
    const float                               ekf_vel_N_accuracy = VelData.north_std_dev;
    const float                               ekf_vel_E_accuracy = VelData.east_std_dev;
    const float                               ekf_vel_D_accuracy = VelData.down_std_dev;

    app::can_tx::VC_VelocityNorth_set(ekf_vel_N);
    app::can_tx::VC_VelocityEast_set(ekf_vel_E);
    app::can_tx::VC_VelocityDown_set(ekf_vel_D);

    app::can_tx::VC_VelocityNorthAccuracy_set(ekf_vel_N_accuracy);
    app::can_tx::VC_VelocityEastAccuracy_set(ekf_vel_E_accuracy);
    app::can_tx::VC_VelocityDownAccuracy_set(ekf_vel_D_accuracy);

    // Velocity
    ekf_solution_mode = (app::can_utils::VcEkfStatus)io::sbgEllipse::getEkfSolutionMode();

    if (static_cast<int>(ekf_solution_mode) < NUM_VC_EKF_STATUS_CHOICES)
    {
        app::can_tx::VC_EkfSolutionMode_set(ekf_solution_mode);
    }

    const io::sbgEllipse::Attitude euler_angles = io::sbgEllipse::getEkfEulerAngles();
    const float                    euler_roll   = euler_angles.roll;
    const float                    euler_pitch  = euler_angles.pitch;
    const float                    euler_yaw    = euler_angles.yaw;

    app::can_tx::VC_EulerAnglesRoll_set(euler_roll);
    app::can_tx::VC_EulerAnglesPitch_set(euler_pitch);
    app::can_tx::VC_EulerAnglesYaw_set(euler_yaw);
}

float getVehicleVelocity(io::sbgEllipse::VelocityData &VelData)
{
    return sqrtf(SQUARE(VelData.north) + SQUARE(VelData.east) + SQUARE(VelData.down));
}

app::can_utils::VcEkfStatus getEkfSolutionMode(void)
{
    return ekf_solution_mode;
}
} // namespace app::sbgEllipse
