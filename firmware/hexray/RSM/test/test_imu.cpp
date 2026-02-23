#include <gtest/gtest.h>
#include "test/test_RSMBase.hpp"
#include "io_imus.hpp"
#include "test_fakes.hpp"
#include "util_errorCodes.hpp"
#include "app_canTx.hpp"

class RSMImuTest : public RSMBaseTest
{
};

TEST_F(RSMImuTest, Accel_Gyro_Test)
{    
    io::imus::imu_rear.set_AccelX(10.0f);
    io::imus::imu_rear.set_AccelY(10.0f);
    io::imus::imu_rear.set_AccelZ(10.0f);
    io::imus::imu_rear.set_GyroRoll(10.0f);
    io::imus::imu_rear.set_GyroPitch(10.0f);
    io::imus::imu_rear.set_GyroYaw(10.0f);

    LetTimePass(100);

    ASSERT_FLOAT_EQ(10.0f, app::can_tx::RSM_AccelX_get());
    ASSERT_FLOAT_EQ(10.0f, app::can_tx::RSM_AccelY_get());
    ASSERT_FLOAT_EQ(10.0f, app::can_tx::RSM_AccelZ_get());
    ASSERT_FLOAT_EQ(10.0f, app::can_tx::RSM_GyroX_get());
    ASSERT_FLOAT_EQ(10.0f, app::can_tx::RSM_GyroY_get());
    ASSERT_FLOAT_EQ(10.0f, app::can_tx::RSM_GyroZ_get());
}