#include "app_imu.hpp"
#include "app_canAlerts.hpp"
#include "app_canTx.hpp"

namespace app::imus
{
using namespace app::can_alerts;

void init()
{
    auto ec1 = IMU1.init();
    auto ec2 = IMU2.init();
    auto ec3 = IMU3.init();
    if (ec1)
        warnings::Imu1InitFailed_set(not ec1.has_value());
    if (ec2)
        warnings::Imu2InitFailed_set(not ec2.has_value());
    if (ec3)
        warnings::Imu3InitFailed_set(not ec3.has_value());
}

void broadcast()
{
    // IMU1 data broadcasting
    const auto accel1 = IMU1.getAccelAll();
    const auto gyro1  = IMU1.getGyroAll();
    // IMU2 data broadcasting
    const auto accel2 = IMU2.getAccelAll();
    const auto gyro2  = IMU2.getGyroAll();
    // IMU3 data broadcasting
    const auto accel3 = IMU3.getAccelAll();
    const auto gyro3  = IMU3.getGyroAll();

    // If either accel or gyro fails for an IMU, set a fault alert
    // for that IMU and skip broadcasting data for that IMU
    if (!accel1 || !gyro1)
        infos::Imu1Fault_set(true);

    // Process data
    app::can_tx::VC_Imu1AccelerationX_set(accel1 ? accel1->x : 0.0f);
    app::can_tx::VC_Imu1AccelerationY_set(accel1 ? accel1->y : 0.0f);
    app::can_tx::VC_Imu1AccelerationZ_set(accel1 ? accel1->z : 0.0f);
    app::can_tx::VC_Imu1AngularVelocityRoll_set(gyro1 ? gyro1->x : 0.0f);
    app::can_tx::VC_Imu1AngularVelocityPitch_set(gyro1 ? gyro1->y : 0.0f);
    app::can_tx::VC_Imu1AngularVelocityYaw_set(gyro1 ? gyro1->z : 0.0f);

    if (!accel2 || !gyro2)
        infos::Imu2Fault_set(true);

    // Process data
    app::can_tx::VC_Imu2AccelerationX_set(accel2 ? accel2->x : 0.0f);
    app::can_tx::VC_Imu2AccelerationY_set(accel2 ? accel2->y : 0.0f);
    app::can_tx::VC_Imu2AccelerationZ_set(accel2 ? accel2->z : 0.0f);

    // Process data
    app::can_tx::VC_Imu2AngularVelocityRoll_set(gyro2 ? gyro2->x : 0.0f);
    app::can_tx::VC_Imu2AngularVelocityPitch_set(gyro2 ? gyro2->y : 0.0f);
    app::can_tx::VC_Imu2AngularVelocityYaw_set(gyro2 ? gyro2->z : 0.0f);

    if (!accel3 || !gyro3)
        infos::Imu3Fault_set(true);

    app::can_tx::VC_Imu3AccelerationX_set(accel3 ? accel3->x : 0.0f);
    app::can_tx::VC_Imu3AccelerationY_set(accel3 ? accel3->y : 0.0f);
    app::can_tx::VC_Imu3AccelerationZ_set(accel3 ? accel3->z : 0.0f);

    app::can_tx::VC_Imu3AngularVelocityRoll_set(gyro3 ? gyro3->x : 0.0f);
    app::can_tx::VC_Imu3AngularVelocityPitch_set(gyro3 ? gyro3->y : 0.0f);
    app::can_tx::VC_Imu3AngularVelocityYaw_set(gyro3 ? gyro3->z : 0.0f);
}

// TODO: state estimation for which IMU is faulty if any and maybe a way to determine if the IMU is giving bad data but
// not fully broken (ex: values are all 0 or not changing at all) and then use the good IMUs to estimate what the bad
// IMU should be outputting and broadcast that instead, and set a fault alert for that IMU as well. Could also use this
// state estimation to determine which IMU data to trust if there is disagreement between the 3 IMUs (ex: 2 similar
// outputs and 1 different output, then take the 2 similar outputs as correct and set a fault for the different one).
} // namespace app::imus