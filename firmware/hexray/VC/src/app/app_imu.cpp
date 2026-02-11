// #include "app_canAlerts.h"
#include "app_imu.hpp"
// Need to pass in spi handle device
// init the spi and extern it in the io layer??
namespace app::imu
{
// {} is init in cpp  accel1{0,0,0}
io::Imu::AccelData accel1{};
io::Imu::AccelData accel2{};
io::Imu::AccelData accel3{};

io::imu::Imu::GyroData gyro1{};
io::imu::Imu::GyroData gyro2{};
io::imu::Imu::GyroData gyro3{};

void app_imu_init() {
    imu1.init();
    imu2.init();
    imu3.init();
}

void app_imu_broadcast_imu()
{
    if (const auto res = imu1.getAccelAll(accel1); not res)
    {
        // handle error
        // REEEFACTORRRR!s
        // app_canTx_VC_ImuAccelerationX_set(0.0f);
        // app_canTx_VC_ImuAccelerationY_set(0.0f);
        // app_canTx_VC_ImuAccelerationZ_set(0.0f);
        // app_canTx_VC_ImuAngularVelocityRoll_set(0.0f);
        // app_canTx_VC_ImuAngularVelocityPitch_set(0.0f);
        // app_canTx_VC_ImuAngularVelocityYaw_set(0.0f);
    }
    else
    {
        // Process data
        // RAHHH REFACTOR!!
        app_canTx_VC_ImuAccelerationX_set(res.data().x);
        app_canTx_VC_ImuAccelerationY_set(res.data().y);
        app_canTx_VC_ImuAccelerationZ_set(res.data().z);
    }

    if (const auto res = imu1.getGyroAll(); not res) {
        // handle error
    } else {
        // app_canTx_VC_ImuAngularVelocityRoll_set(0.0f);
        // app_canTx_VC_ImuAngularVelocityPitch_set(0.0f);
        // app_canTx_VC_ImuAngularVelocityYaw_set(0.0f);
    }

    if (imu2.getAccelAll(&accel2) != EXIT_CODE_OK)
    {
        // handle error
        // RAHHHH REGACTOR
        // app_canTx_VC_ImuAccelerationX_set(0.0f);
        // app_canTx_VC_ImuAccelerationY_set(0.0f);
        // app_canTx_VC_ImuAccelerationZ_set(0.0f);
        // app_canTx_VC_ImuAngularVelocityRoll_set(0.0f);
        // app_canTx_VC_ImuAngularVelocityPitch_set(0.0f);
        // app_canTx_VC_ImuAngularVelocityYaw_set(0.0f);
    }
    else
    {
        // Process data
        static float *lat_accel2  = accel2.x;
        static float *long_accel2 = accel2.y;
        static float *z_accel2    = accel2.z;
        static float *roll_rate2  = gyro2.x;
        static float *pitch_rate2 = gyro2.y;
        static float *yaw_rate2   = gyro2.z;
        // REFACTOR!!!
        // app_canTx_VC_ImuAccelerationX_set(&lat_accel2);
        // app_canTx_VC_ImuAccelerationY_set(&long_accel2);
        // app_canTx_VC_ImuAccelerationZ_set(&z_accel2);
        // app_canTx_VC_ImuAngularVelocityRoll_set(&roll_rate2);
        // app_canTx_VC_ImuAngularVelocityPitch_set(&pitch_rate2);
        // app_canTx_VC_ImuAngularVelocityYaw_set(&yaw_rate2);
    }

    if (imu3.getAccelAll(&accel3) != EXIT_CODE_OK)
    {
        // handle error
        // REFACTOR!!!
        // app_canTx_VC_ImuAccelerationX_set(0.0f);
        // app_canTx_VC_ImuAccelerationY_set(0.0f);
        // app_canTx_VC_ImuAccelerationZ_set(0.0f);
        // app_canTx_VC_ImuAngularVelocityRoll_set(0.0f);
        // app_canTx_VC_ImuAngularVelocityPitch_set(0.0f);
        // app_canTx_VC_ImuAngularVelocityYaw_set(0.0f);
    }
    else
    {
        // Process data
        static float *lat_accel3  = accel3.x;
        static float *long_accel3 = accel3.y;
        static float *z_accel3    = accel3.z;
        static float *roll_rate3  = gyro3.x;
        static float *pitch_rate3 = gyro3.y;
        static float *yaw_rate3   = gyro3.z;
        //  refactor
        // app_canTx_VC_ImuAccelerationX_set(&lat_accel3);
        // app_canTx_VC_ImuAccelerationY_set(&long_accel3);
        // app_canTx_VC_ImuAccelerationZ_set(&z_accel3);
        // app_canTx_VC_ImuAngularVelocityRoll_set(&roll_rate3);
        // app_canTx_VC_ImuAngularVelocityPitch_set(&pitch_rate3);
        // app_canTx_VC_ImuAngularVelocityYaw_set(&yaw_rate3);
    }
}

const io::Imu::AccelData *app_imu_getAccelData(io::Imu::AccelData &data)
{
    return imu1.;
}

const io::Imu::GyroData& app_imu_getGyroData(io::Imu::GyroData &data)
{
    return data;
}



// ask what we wanna do with the 3 imu logic?
// IDEA1: get the average of the 3
// IDEA 2: a sm with the following states
// all agree, all disgree, 2 agree one disagree if all agree/disagree averge otherwise take the average of the 2
// agreeing ones or take the middle   of the two agreeing ones const ImuFaults *app_imu_getFaultData()
// {
//     return &imu_faults;
// }
} // namespace app::imu