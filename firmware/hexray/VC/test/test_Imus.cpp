#include <gtest/gtest.h>
#include "test/test_VCBase.hpp"
// #include "io_imus.hpp"
#include "io_imu.hpp"
#include "app_imu.hpp"

#include "vc_fakes.hpp"
#include "util_errorCodes.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"

class VCImuTest : public VCBaseTest
{
};

// IMU1
TEST_F(VCImuTest, Imu1_Accel_Gyro_Test)
{
    IMU1.set_AccelX(1.0f);
    IMU1.set_AccelY(2.0f);
    IMU1.set_AccelZ(3.0f);
    IMU1.set_GyroRoll(4.0f);
    IMU1.set_GyroPitch(5.0f);
    IMU1.set_GyroYaw(6.0f);

    LetTimePass(1);

    app::imus::broadcast();

    LetTimePass(1);

    ASSERT_FLOAT_EQ(1.0f, app::can_tx::VC_Imu1AccelerationX_get());
    ASSERT_FLOAT_EQ(2.0f, app::can_tx::VC_Imu1AccelerationY_get());
    ASSERT_FLOAT_EQ(3.0f, app::can_tx::VC_Imu1AccelerationZ_get());
    ASSERT_FLOAT_EQ(4.0f, app::can_tx::VC_Imu1AngularVelocityRoll_get());
    ASSERT_FLOAT_EQ(5.0f, app::can_tx::VC_Imu1AngularVelocityPitch_get());
    ASSERT_FLOAT_EQ(6.0f, app::can_tx::VC_Imu1AngularVelocityYaw_get());
}

// IMU2
TEST_F(VCImuTest, Imu2_Accel_Gyro_Test)
{
    IMU2.set_AccelX(1.0f);
    IMU2.set_AccelY(2.0f);
    IMU2.set_AccelZ(3.0f);
    IMU2.set_GyroRoll(4.0f);
    IMU2.set_GyroPitch(5.0f);
    IMU2.set_GyroYaw(6.0f);

    LetTimePass(1);
    app::imus::broadcast();
    LetTimePass(1);

    ASSERT_FLOAT_EQ(1.0f, app::can_tx::VC_Imu2AccelerationX_get());
    ASSERT_FLOAT_EQ(2.0f, app::can_tx::VC_Imu2AccelerationY_get());
    ASSERT_FLOAT_EQ(3.0f, app::can_tx::VC_Imu2AccelerationZ_get());
    ASSERT_FLOAT_EQ(4.0f, app::can_tx::VC_Imu2AngularVelocityRoll_get());
    ASSERT_FLOAT_EQ(5.0f, app::can_tx::VC_Imu2AngularVelocityPitch_get());
    ASSERT_FLOAT_EQ(6.0f, app::can_tx::VC_Imu2AngularVelocityYaw_get());
}

// IMU3

TEST_F(VCImuTest, Imu3_Accel_Gyro_Test)
{
    IMU3.set_AccelX(1.0f);
    IMU3.set_AccelY(2.0f);
    IMU3.set_AccelZ(3.0f);
    IMU3.set_GyroRoll(4.0f);
    IMU3.set_GyroPitch(5.0f);
    IMU3.set_GyroYaw(6.0f);

    LetTimePass(1);
    app::imus::broadcast();
    LetTimePass(1);

    ASSERT_FLOAT_EQ(1.0f, app::can_tx::VC_Imu3AccelerationX_get());
    ASSERT_FLOAT_EQ(2.0f, app::can_tx::VC_Imu3AccelerationY_get());
    ASSERT_FLOAT_EQ(3.0f, app::can_tx::VC_Imu3AccelerationZ_get());
    ASSERT_FLOAT_EQ(4.0f, app::can_tx::VC_Imu3AngularVelocityRoll_get());
    ASSERT_FLOAT_EQ(5.0f, app::can_tx::VC_Imu3AngularVelocityPitch_get());
    ASSERT_FLOAT_EQ(6.0f, app::can_tx::VC_Imu3AngularVelocityYaw_get());
}

// ERROR
TEST_F(VCImuTest, Imu1_Accel_Error_Returns_Early)
{
    IMU1.imu_status_set(false);
    IMU2.imu_status_set(true);
    IMU3.imu_status_set(true);
    LetTimePass(1);
    app::imus::init();
    LetTimePass(1);

    // CAN values should remain at default (0.0f)
    ASSERT_FLOAT_EQ(0.0f, app::can_tx::VC_Warning_Imu1InitFailed_get());
    ASSERT_FLOAT_EQ(0.0f, app::can_tx::VC_Warning_Imu2InitFailed_get());
    ASSERT_FLOAT_EQ(0.0f, app::can_tx::VC_Warning_Imu3InitFailed_get());
}