// #include "app_canAlerts.h"
#include "app_imu.hpp"
// Need to pass in spi handle device
// init the spi and extern it in the io layer??
constexpr explicit hw::spi::SpiDevice imu_spi1(SpiBus &bus_in, const Gpio &nss_in, const uint32_t timeoutMs_in);
constexpr explicit hw::spi::SpiDevice imu_spi2();
constexpr explicit hw::spi::SpiDevice imu_spi3();

constexpr explicit io::imu::Imu imu1(imu_spi1);
constexpr explicit io::imu::Imu imu2(imu_spi2);
constexpr explicit io::imu::Imu imu3(imu_spi3);

imu1().init();
imu2().init();
imu3().init();

// {} is init in cpp
io::imu::Imu::AccelData accel1{};
io::imu::Imu::AccelData accel2{};
io::imu::Imu::AccelData accel3{};

io::imu::Imu::GyroData gyro1{};
io::imu::Imu::GyroData gyro2{};
io::imu::Imu::GyroData gyro3{};

void app_imu_broadcast_imu()
{
    if (imu1.getAccelAll(&accel1) != ExitCode::EXIT_CODE_OK)
    {
        // handle error
        app_canTx_VC_ImuAccelerationX_set(0.0f);
        app_canTx_VC_ImuAccelerationY_set(0.0f);
        app_canTx_VC_ImuAccelerationZ_set(0.0f);
        app_canTx_VC_ImuAngularVelocityRoll_set(0.0f);
        app_canTx_VC_ImuAngularVelocityPitch_set(0.0f);
        app_canTx_VC_ImuAngularVelocityYaw_set(0.0f);
    }
    else
    {
        // Process data
        static float *lat_accel1  = imu1.accel1.x;
        static float *long_accel1 = imu1.accel1.y;
        static float *z_accel1    = imu1.accel1.z;
        static float *roll_rate1  = imu1.gyro.x;
        static float *pitch_rate1 = imu1.gyro.y;
        static float *yaw_rate1   = imu1.gyro.z;
        app_canTx_VC_ImuAccelerationX_set(&lat_accel1);
        app_canTx_VC_ImuAccelerationY_set(&long_accel1);
        app_canTx_VC_ImuAccelerationZ_set(&z_accel1);
        app_canTx_VC_ImuAngularVelocityRoll_set(&roll_rate1);
        app_canTx_VC_ImuAngularVelocityPitch_set(&pitch_rate1);
        app_canTx_VC_ImuAngularVelocityYaw_set(&yaw_rate1);
    }

    if (imu2.getAccelAll(&accel2) != ExitCode::EXIT_CODE_OK)
    {
        // handle error
        app_canTx_VC_ImuAccelerationX_set(0.0f);
        app_canTx_VC_ImuAccelerationY_set(0.0f);
        app_canTx_VC_ImuAccelerationZ_set(0.0f);
        app_canTx_VC_ImuAngularVelocityRoll_set(0.0f);
        app_canTx_VC_ImuAngularVelocityPitch_set(0.0f);
        app_canTx_VC_ImuAngularVelocityYaw_set(0.0f);
    }
    else
    {
        // Process data
        static float *lat_accel2  = imu2.accel2.x;
        static float *long_accel2 = imu2.accel2.y;
        static float *z_accel2    = imu2.accel2.z;
        static float *roll_rate2  = imu2.gyro.x;
        static float *pitch_rate2 = imu2.gyro.y;
        static float *yaw_rate2   = imu2.gyro.z;
        app_canTx_VC_ImuAccelerationX_set(&lat_accel2);
        app_canTx_VC_ImuAccelerationY_set(&long_accel2);
        app_canTx_VC_ImuAccelerationZ_set(&z_accel2);
        app_canTx_VC_ImuAngularVelocityRoll_set(&roll_rate2);
        app_canTx_VC_ImuAngularVelocityPitch_set(&pitch_rate2);
        app_canTx_VC_ImuAngularVelocityYaw_set(&yaw_rate2);
    }

    if (imu3.getAccelAll(&accel3) != ExitCode::EXIT_CODE_OK)
    {
        // handle error
        app_canTx_VC_ImuAccelerationX_set(0.0f);
        app_canTx_VC_ImuAccelerationY_set(0.0f);
        app_canTx_VC_ImuAccelerationZ_set(0.0f);
        app_canTx_VC_ImuAngularVelocityRoll_set(0.0f);
        app_canTx_VC_ImuAngularVelocityPitch_set(0.0f);
        app_canTx_VC_ImuAngularVelocityYaw_set(0.0f);
    }
    else
    {
        // Process data
        static float *lat_accel3  = imu3.accel3.x;
        static float *long_accel3 = imu3.accel3.y;
        static float *z_accel3    = imu3.accel3.z;
        static float *roll_rate3  = imu3.gyro.x;
        static float *pitch_rate3 = imu3.gyro.y;
        static float *yaw_rate3   = imu3.gyro.z;
        app_canTx_VC_ImuAccelerationX_set(&lat_accel3);
        app_canTx_VC_ImuAccelerationY_set(&long_accel3);
        app_canTx_VC_ImuAccelerationZ_set(&z_accel3);
        app_canTx_VC_ImuAngularVelocityRoll_set(&roll_rate3);
        app_canTx_VC_ImuAngularVelocityPitch_set(&pitch_rate3);
        app_canTx_VC_ImuAngularVelocityYaw_set(&yaw_rate3);
    }
}

const AccelData *app_imu_getAccelData(AccelData &data)
{
    return data;
}

const GyroData *app_imu_getGyroData(GyroData &data)
{
    return data;
}

// const ImuFaults *app_imu_getFaultData()
// {
//     return &imu_faults;
// }
