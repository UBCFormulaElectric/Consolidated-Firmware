#include "test_VCBase.hpp"
#include <gtest/gtest.h>
#include "util_errorCodes.hpp"
#include "app_canRx.hpp"
#include "app_imu.cpp"
#include "app_canTx.hpp"


class VCTestImu: public VCBaseTest
{
};
TEST_F(VCTestImu, broadcastImuTest)
{
    // fakes::io::sbgEllipses::setLinearAcceleration(1.0f, 2.0f, 3.0f);

    // app::imu::broadcast_imu();

    // EXPECT_FLOAT_EQ(app::can_tx::VC_Imu1AccelerationX_get(), 1.0f);
    // EXPECT_FLOAT_EQ(app::can_tx::VC_Imu1AccelerationY_get(), 2.0f);
    // EXPECT_FLOAT_EQ(app::can_tx::VC_Imu1AccelerationZ_get(), 3.0f);
}
