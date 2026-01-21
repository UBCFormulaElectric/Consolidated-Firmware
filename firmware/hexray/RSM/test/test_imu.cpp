#include <gtest/gtest.h>
#include "test/test_RSMBase.hpp"
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
    extern io::imu::Imu imu_config; 

    fakes::io::imu::setAccelX(10, ExitCode::EXIT_CODE_OK);
    fakes::io::imu::setAccelY(10, ExitCode::EXIT_CODE_OK);
    fakes::io::imu::setAccelZ(10, ExitCode::EXIT_CODE_OK);
    fakes::io::imu::setGyroX(10, ExitCode::EXIT_CODE_OK);
    fakes::io::imu::setGyroY(10, ExitCode::EXIT_CODE_OK);
    fakes::io::imu::setGyroZ(10, ExitCode::EXIT_CODE_OK);

    LetTimePass(100);

    EXPECT_EQ(10, app_canTx_RSM_LinearAccelerationInX_get());
    EXPECT_EQ(10, app_canTx_RSM_LinearAccelerationInY_get());
    EXPECT_EQ(10, app_canTx_RSM_LinearAccelerationInZ_get());
    EXPECT_EQ(10, app_canTx_RSM_RollAngularAcceleration_get());
    EXPECT_EQ(10, app_canTx_RSM_PitchAngularAcceleration_get());
    EXPECT_EQ(10, app_canTx_RSM_YawAngularAcceleration_get());
}