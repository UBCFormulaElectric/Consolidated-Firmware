
#include "app_sbgEllipse.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canAlerts.hpp"
#include "util_utils.hpp"
#include "util_errorCodes.hpp"
#include "app_math.hpp"

namespace app::sbgEllipse
{
static can_utils::VcEkfStatus         ekf_solution_mode = can_utils::VcEkfStatus::UNINITIALIZED;
static constexpr int                  NUM_VC_EKF_STATUS_CHOICES{ 5 };
static std::expected<void, ErrorCode> sbg_init_ok = std::unexpected(ErrorCode::ERROR);

void init()
{
    sbg_init_ok = io::sbgEllipse::init();
    can_alerts::infos::SbgInitFailed_set(not sbg_init_ok.has_value());
}

void broadcast()
{
    /* Enable these back when you turn this on in the SBG, otherwise it's still sending
       CAN messages because another message in the signal is being used */

    // Status msg
    can_tx::VC_EllipseGeneralStatusBitmask_set(io::sbgEllipse::getGeneralStatus());
    can_tx::VC_EllipseComStatusBitmask_set(io::sbgEllipse::getComStatus());
    can_tx::VC_EllipseQueueOverflowCount_set(io::sbgEllipse::getOverflowCount());

    // Time msg
    uint32_t timestamp_us = io::sbgEllipse::getTimestampUs();
    can_tx::VC_EllipseTimestamp_set(timestamp_us);

    // EKF
    const static io::sbgEllipse::VelocityData VelData            = io::sbgEllipse::getEkfNavVelocityData();
    float                                     ekf_vel_N          = VelData.north;
    float                                     ekf_vel_E          = VelData.east;
    float                                     ekf_vel_D          = VelData.down;
    const float                               ekf_vel_N_accuracy = VelData.north_std_dev;
    const float                               ekf_vel_E_accuracy = VelData.east_std_dev;
    const float                               ekf_vel_D_accuracy = VelData.down_std_dev;

    can_tx::VC_VelocityNorth_set(ekf_vel_N);
    can_tx::VC_VelocityEast_set(ekf_vel_E);
    can_tx::VC_VelocityDown_set(ekf_vel_D);

    can_tx::VC_VelocityNorthAccuracy_set(ekf_vel_N_accuracy);
    can_tx::VC_VelocityEastAccuracy_set(ekf_vel_E_accuracy);
    can_tx::VC_VelocityDownAccuracy_set(ekf_vel_D_accuracy);

    // Velocity
    ekf_solution_mode = (can_utils::VcEkfStatus)io::sbgEllipse::getEkfSolutionMode();

    if (static_cast<int>(ekf_solution_mode) < NUM_VC_EKF_STATUS_CHOICES)
    {
        can_tx::VC_EkfSolutionMode_set(ekf_solution_mode);
    }

    const io::sbgEllipse::Attitude euler_angles = io::sbgEllipse::getEkfEulerAngles();
    const float                    euler_roll   = euler_angles.roll;
    const float                    euler_pitch  = euler_angles.pitch;
    const float                    euler_yaw    = euler_angles.yaw;

    can_tx::VC_EulerAnglesRoll_set(euler_roll);
    can_tx::VC_EulerAnglesPitch_set(euler_pitch);
    can_tx::VC_EulerAnglesYaw_set(euler_yaw);
}

float getVehicleVelocity(io::sbgEllipse::VelocityData &VelData)
{
    return sqrtf(SQUARE(VelData.north) + SQUARE(VelData.east) + SQUARE(VelData.down));
}

bool sbgInitOk()
{
    return sbg_init_ok.has_value();
}

can_utils::VcEkfStatus getEkfSolutionMode(void)
{
    return ekf_solution_mode;
}
} // namespace app::sbgEllipse
