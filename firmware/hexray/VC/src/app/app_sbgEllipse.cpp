
#include "app_sbgEllipse.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canAlerts.hpp"
#include "util_utils.hpp"
#include "util_errorCodes.hpp"
#include "app_math.hpp"

namespace app::sbgEllipse
{
static can_utils::VcEkfStatus ekf_solution_mode = can_utils::VcEkfStatus::UNINITIALIZED;
static constexpr int          NUM_VC_EKF_STATUS_CHOICES{ 5 };
static result<void>           sbg_init_ok = std::unexpected(ErrorCode::ERROR);

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
    const io::sbgEllipse::VelocityData VelData = io::sbgEllipse::getEkfNavVelocityData();

    can_tx::VC_VelocityNorth_set(VelData.north);
    can_tx::VC_VelocityEast_set(VelData.east);
    can_tx::VC_VelocityDown_set(VelData.down);

    can_tx::VC_VelocityNorthAccuracy_set(VelData.north_std_dev);
    can_tx::VC_VelocityEastAccuracy_set(VelData.east_std_dev);
    can_tx::VC_VelocityDownAccuracy_set(VelData.down_std_dev);

    can_tx::VC_VelocityX_set(VelData.north);
    can_tx::VC_VelocityY_set(VelData.east);
    can_tx::VC_VelocityZ_set(VelData.down);

    can_tx::VC_VelocityXAccuracy_set(VelData.north_std_dev);
    can_tx::VC_VelocityYAccuracy_set(VelData.east_std_dev);
    can_tx::VC_VelocityZAccuracy_set(VelData.down_std_dev);

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

float bodyVelX()
{
    return io::sbgEllipse::getEkfNavVelocityData().vel_x;
}

float bodyVelY()
{
    return io::sbgEllipse::getEkfNavVelocityData().vel_y;
}

float bodyVelZ()
{
    return io::sbgEllipse::getEkfNavVelocityData().vel_z;
}

float globalVelN()
{
    return io::sbgEllipse::getEkfNavVelocityData().north;
}

float globalVelE()
{
    return io::sbgEllipse::getEkfNavVelocityData().east;
}

float globalVelD()
{
    return io::sbgEllipse::getEkfNavVelocityData().down;
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
