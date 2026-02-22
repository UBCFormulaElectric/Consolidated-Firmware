#include <gtest/gtest.h>
#include "test/test_RSMBase.hpp"
#include "io_imus.hpp"
#include "test_fakes.hpp"
#include "util_errorCodes.hpp"
#include "app_canTx.hpp"

class RSMImuTest : public RSMBaseTest
{
};

TEST_F(RSMImuTest, Acceleration_Velocity_Test)
{
    imu_config.set_AccelX(10);
    imu_config.set_AccelY(10);
    imu_config.set_AccelZ(10);
    imu_config.set_GyroRoll(10);
    imu_config.set_GyroPitch(10);
    imu_config.set_GyroYaw(10);

    LetTimePass(100);

    EXPECT_EQ(10, app::can_tx::RSM_LinearAccelerationInX_get());
    EXPECT_EQ(10, app::can_tx::RSM_LinearAccelerationInY_get());
    EXPECT_EQ(10, app::can_tx::RSM_LinearAccelerationInZ_get());
    EXPECT_EQ(10, app::can_tx::RSM_RollAngularAcceleration_get());
    EXPECT_EQ(10, app::can_tx::RSM_PitchAngularAcceleration_get());
    EXPECT_EQ(10, app::can_tx::RSM_YawAngularAcceleration_get());
}