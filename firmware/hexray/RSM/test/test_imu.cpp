#include <gtest/gtest.h>
#include "test/test_RSMBase.hpp"
#include "test_fakes.hpp"

extern "C"
{
    #include "app_canTx.h"
}

class RSMImuTest : public RSMBaseTest
{};

TEST_F(RSMImuTest, Acceleration_Velocity_Test)
{
    fakes::io::imu::setLinearAccelerationX(10);
    fakes::io::imu::setLinearAccelerationY(10);
    fakes::io::imu::setLinearAccelerationZ(10);
    fakes::io::imu::setAngularVelocityRoll(10);
    fakes::io::imu::setAngularVelocityPitch(10);
    fakes::io::imu::setAngularVelocityYaw(10);

    LetTimePass(10);

    EXPECT_EQ(10, app_canTx_RSM_LinearAccelerationInX_get());
    EXPECT_EQ(10, app_canTx_RSM_LinearAccelerationInY_get());
    EXPECT_EQ(10, app_canTx_RSM_LinearAccelerationInZ_get());
    EXPECT_EQ(10, app_canTx_RSM_RollAngularAcceleration_get());
    EXPECT_EQ(10, app_canTx_RSM_PitchAngularAcceleration_get());
    EXPECT_EQ(10, app_canTx_RSM_YawAngularAcceleration_get());
}