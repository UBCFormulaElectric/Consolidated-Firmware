#include <gtest/gtest.h>
#include "test/test_FSMBase.hpp"
#include "util_errorCodes.hpp"
#include "app_canTx.hpp"
#include "io_imus.hpp"

extern "C"
{
}

class FSMImuTest : public FSMBaseTest
{
};

TEST_F(FSMImuTest, Acceleration_Velocity_Test)
{
    imu_config.set_AccelX(10);
    imu_config.set_AccelY(10);
    imu_config.set_AccelZ(10);
    imu_config.set_GyroRoll(10);
    imu_config.set_GyroPitch(10);
    imu_config.set_GyroYaw(10);

    LetTimePass(100);

    EXPECT_EQ(10, app::can_tx::FSM_LinearAccelerationInX_get());
    EXPECT_EQ(10, app::can_tx::FSM_LinearAccelerationInY_get());
    EXPECT_EQ(10, app::can_tx::FSM_LinearAccelerationInZ_get());
    EXPECT_EQ(10, app::can_tx::FSM_RollAngularAcceleration_get());
    EXPECT_EQ(10, app::can_tx::FSM_PitchAngularAcceleration_get());
    EXPECT_EQ(10, app::can_tx::FSM_YawAngularAcceleration_get());
}