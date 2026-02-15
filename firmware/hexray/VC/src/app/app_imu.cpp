#include "app_imu.hpp"
#include "app_canAlerts.hpp"
#include "app_canTx.hpp"

namespace app::imu
{

void broadcast_imu()
{
    // IMU1 data broadcasting
    if (const auto accel = imu1.getAccelAll(); not accel)
    {
        // handle error
        return std::unexpected(ErrorCode::ERROR);
    }
    else
    {
        // Process data
        app::can_tx::VC_Imu1AccelerationX_set(accel.data().x);
        app::can_tx::VC_Imu1AccelerationY_set(accel.data().y);
        app::can_tx::VC_Imu1AccelerationZ_set(accel.data().z);
    }
    if (const auto gyro = imu1.getGyroAll(); not gyro)
    {
        return std::unexpected(ErrorCode::ERROR);
    }
    else
    {
        app::can_tx::VC_Imu1AngularVelocityRoll_set(gyro.data().x);
        app::can_tx::VC_Imu1AngularVelocityPitch_set(gyro.data().y);
        app::can_tx::VC_Imu1AngularVelocityYaw_set(gyro.data().z);
    }

    // IMU2 data broadcasting
    if (const auto accel = imu2.getAccelAll(); not accel)
    {
        return std::unexpected(ErrorCode::ERROR);
    }
    else
    {
        // Process data
        app::can_tx::VC_Imu2AccelerationX_set(accel.data().x);
        app::can_tx::VC_Imu2AccelerationY_set(accel.data().y);
        app::can_tx::VC_Imu2AccelerationZ_set(accel.data().z);
    }
    if (const auto gyro = imu2.getGyroAll(); not gyro)
    {
        return std::unexpected(ErrorCode::ERROR);
    }
    else
    {
        // Process data
        app::can_tx::VC_Imu2AngularVelocityRoll_set(gyro.data().x);
        app::can_tx::VC_Imu2AngularVelocityPitch_set(gyro.data().y);
        app::can_tx::VC_Imu2AngularVelocityYaw_set(gyro.data().z);
    }

    // IMU3 data broadcasting
    if (const auto accel = imu3.getAccelAll(); not accel)
    {
        return std::unexpected(ErrorCode::ERROR);
    }
    else
    {
        app::can_tx::VC_Imu3AccelerationX_set(accel.data().x);
        app::can_tx::VC_Imu3AccelerationY_set(accel.data().y);
        app::can_tx::VC_Imu3AccelerationZ_set(accel.data().z);
    }
    if (const auto gyro = imu3.getGyroAll(); not gyro)
    {
        return std::unexpected(ErrorCode::ERROR);
    }
    else
    {
        app::can_tx::VC_Imu3AngularVelocityRoll_set(gyro.data().x);
        app::can_tx::VC_Imu3AngularVelocityPitch_set(gyro.data().y);
        app::can_tx::VC_Imu3AngularVelocityYaw_set(gyro.data().z);
    }
}

// prob donty need ts no more
// const auto &getAccelData(io::Imu imu)
// {
//     return const auto accel = imu.getAccelAll();
// }

// const auto &getGyroData(io::Imu imu)
// {
//     return const auto accel = imu.getGyroAll();
// }

// ask what we wanna do with the 3 imu logic?
// IDEA1: get the average of the 3
// IDEA 2: a sm with the following states
// all agree, all disgree, 2 agree one disagree if all agree/disagree averge otherwise take the average of the 2
// agreeing ones or take the middle   of the two agreeing ones const ImuFaults *app_imu_getFaultData()
// {
//     return &imu_faults;
// }
} // namespace app::imu