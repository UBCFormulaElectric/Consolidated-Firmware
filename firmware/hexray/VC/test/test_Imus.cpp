#include <gtest/gtest.h>
#include "test/test_VCBase.hpp"
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
    io::imus::IMU1.set_AccelX(1.0f);
    io::imus::IMU1.set_AccelY(2.0f);
    io::imus::IMU1.set_AccelZ(3.0f);
    io::imus::IMU1.set_GyroRoll(4.0f);
    io::imus::IMU1.set_GyroPitch(5.0f);
    io::imus::IMU1.set_GyroYaw(6.0f);

    LetTimePass(1);

    app::imus::broadcast();

    LetTimePass(1);

    ASSERT_FLOAT_EQ(1.0f, app::can_tx::VC_Imu1AccelX_get());
    ASSERT_FLOAT_EQ(2.0f, app::can_tx::VC_Imu1AccelY_get());
    ASSERT_FLOAT_EQ(3.0f, app::can_tx::VC_Imu1AccelZ_get());
    ASSERT_FLOAT_EQ(4.0f, app::can_tx::VC_Imu1GyroX_get());
    ASSERT_FLOAT_EQ(5.0f, app::can_tx::VC_Imu1GyroY_get());
    ASSERT_FLOAT_EQ(6.0f, app::can_tx::VC_Imu1GyroZ_get());
}

// ERROR
TEST_F(VCImuTest, Imu1_Accel_Error_Returns_Early)
{
    io::imus::IMU1.imu_status_set(false);
    LetTimePass(1);
    app::imus::init();
    LetTimePass(1);

    // CAN values should remain at default (0.0f)
    ASSERT_EQ(true, app::can_tx::VC_Info_Imu1InitFailed_get());
}