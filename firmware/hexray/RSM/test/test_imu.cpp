#include <gtest/gtest.h>
#include "test/test_RSMBase.hpp"
<<<<<<< HEAD
=======
#include "io_imu.hpp"
#include "app_imu.hpp"
>>>>>>> master
#include "test_fakes.hpp"
#include "util_errorCodes.hpp"

extern "C"
{
#include "app_canTx.h"
}

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

    EXPECT_EQ(10, app_canTx_RSM_LinearAccelerationInX_get());
    EXPECT_EQ(10, app_canTx_RSM_LinearAccelerationInY_get());
    EXPECT_EQ(10, app_canTx_RSM_LinearAccelerationInZ_get());
    EXPECT_EQ(10, app_canTx_RSM_RollAngularAcceleration_get());
    EXPECT_EQ(10, app_canTx_RSM_PitchAngularAcceleration_get());
    EXPECT_EQ(10, app_canTx_RSM_YawAngularAcceleration_get());
}