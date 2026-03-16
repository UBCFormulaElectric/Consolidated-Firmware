#include "app_imu.hpp"
#include "app_canAlerts.hpp"
#include "app_canTx.hpp"

namespace app::imu
{
using namespace app::can_alerts;

void init()
{
    auto ec = io::imus::init();
    warnings::ImuInitFailed_set(not ec.has_value());
}

void broadcast()
{
    // IMU1 data broadcasting
    const auto accel1 = imu1.getAccelAll();
    const auto gyro1  = imu1.getGyroAll();

    // If either accel or gyro fails for an IMU, set a fault alert
    // for that IMU and skip broadcasting data for that IMU
    if (!accel1 || !gyro1)
    {
        // handle error by setting an alert or could also seperate into 3 broadcasting
        // fuunctions to return a fault for each IMU if not working.
        infos::Imu1Fault_set(true);
    }
    if (accel1)
    {
        // Process data
        app::can_tx::VC_Imu1AccelerationX_set(accel1.data().x);
        app::can_tx::VC_Imu1AccelerationY_set(accel1.data().y);
        app::can_tx::VC_Imu1AccelerationZ_set(accel1.data().z);
    }
    if (gyro1)
    {
        app::can_tx::VC_Imu1AngularVelocityRoll_set(gyro1.data().x);
        app::can_tx::VC_Imu1AngularVelocityPitch_set(gyro1.data().y);
        app::can_tx::VC_Imu1AngularVelocityYaw_set(gyro1.data().z);
    }

    // IMU2 data broadcasting
    const auto accel2 = imu2.getAccelAll();
    const auto gyro2  = imu2.getGyroAll();

    if (!accel2 || !gyro2)
    {
        infos::Imu2Fault_set(true);
    }
    if (accel2)
    {
        // Process data
        app::can_tx::VC_Imu2AccelerationX_set(accel2.data().x);
        app::can_tx::VC_Imu2AccelerationY_set(accel2.data().y);
        app::can_tx::VC_Imu2AccelerationZ_set(accel2.data().z);
    }
    if (gyro2)
    {
        // Process data
        app::can_tx::VC_Imu2AngularVelocityRoll_set(gyro2.data().x);
        app::can_tx::VC_Imu2AngularVelocityPitch_set(gyro2.data().y);
        app::can_tx::VC_Imu2AngularVelocityYaw_set(gyro.data().z);
    }

    // IMU3 data broadcasting
    const auto accel3 = imu3.getAccelAll();
    const auto gyro3  = imu3.getGyroAll();

    if (!accel3 || !gyro3)
    {
        infos::Imu3Fault_set(true);
    }
    if (accel3)
    {
        app::can_tx::VC_Imu3AccelerationX_set(accel3.data().x);
        app::can_tx::VC_Imu3AccelerationY_set(accel3.data().y);
        app::can_tx::VC_Imu3AccelerationZ_set(accel3.data().z);
    }
    if (gyro3)
    {
        app::can_tx::VC_Imu3AngularVelocityRoll_set(gyro3.data().x);
        app::can_tx::VC_Imu3AngularVelocityPitch_set(gyro3.data().y);
        app::can_tx::VC_Imu3AngularVelocityYaw_set(gyro3.data().z);
    }
}

// TODO: 3 imu fusion
// IDEA: a SM with the following states
// all agree, all disgree, 2 agree one disagree if all agree/disagree averge otherwise take the average of the 2
// agreeing ones or take the middle   of the two agreeing ones const ImuFaults *app_imu_getFaultData()
// {
//     return &imu_faults;
// }
} // namespace app::imu