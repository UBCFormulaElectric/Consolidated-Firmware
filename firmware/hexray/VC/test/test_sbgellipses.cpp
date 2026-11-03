#include <gtest/gtest.h>
#include "test/test_VCBase.hpp"
#include "app_sbgEllipse.hpp"
#include "vc_fakes.hpp"
#include "util_errorCodes.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"

class VCSbgEllipsesTest : public VCBaseTest
{
};

TEST_F(VCSbgEllipsesTest, test_broadcast)
{
    fakes::io::sbgEllipse::setGeneralStatus(0x1234u);
    fakes::io::sbgEllipse::setComStatus(0x56789ABCu);
    fakes::io::sbgEllipse::setOverflowCount(7u);
    fakes::io::sbgEllipse::setTimestampUs(424242u);
    fakes::io::sbgEllipse::setVelocity(
        9u, 1.25f, -2.5f, 3.75f, 0.1f, 0.2f, 0.3f, 10.5f, -1.75f, 0.25f, 0.4f, 0.5f, 0.6f);
    fakes::io::sbgEllipse::setSolutionMode(static_cast<uint32_t>(app::can_utils::VcEkfStatus::AHRS));
    fakes::io::sbgEllipse::setAttitude(0.4f, -0.5f, 1.6f);

    app::sbgEllipse::broadcast();

    ASSERT_EQ(0x1234u, app::can_tx::VC_EllipseGeneralStatusBitmask_get());
    ASSERT_EQ(0x56789ABCu, app::can_tx::VC_EllipseComStatusBitmask_get());
    ASSERT_EQ(7u, app::can_tx::VC_EllipseQueueOverflowCount_get());
    ASSERT_EQ(424242u, app::can_tx::VC_EllipseTimestamp_get());
    ASSERT_FLOAT_EQ(1.25f, app::can_tx::VC_VelocityNorth_get());
    ASSERT_FLOAT_EQ(-2.5f, app::can_tx::VC_VelocityEast_get());
    ASSERT_FLOAT_EQ(3.75f, app::can_tx::VC_VelocityDown_get());
    ASSERT_FLOAT_EQ(0.1f, app::can_tx::VC_VelocityNorthAccuracy_get());
    ASSERT_FLOAT_EQ(0.2f, app::can_tx::VC_VelocityEastAccuracy_get());
    ASSERT_FLOAT_EQ(0.3f, app::can_tx::VC_VelocityDownAccuracy_get());
    ASSERT_FLOAT_EQ(10.5f, app::can_tx::VC_VelocityX_get());
    ASSERT_FLOAT_EQ(-1.75f, app::can_tx::VC_VelocityY_get());
    ASSERT_FLOAT_EQ(0.25f, app::can_tx::VC_VelocityZ_get());
    ASSERT_FLOAT_EQ(0.4f, app::can_tx::VC_VelocityXAccuracy_get());
    ASSERT_FLOAT_EQ(0.5f, app::can_tx::VC_VelocityYAccuracy_get());
    ASSERT_FLOAT_EQ(0.6f, app::can_tx::VC_VelocityZAccuracy_get());
    ASSERT_EQ(app::can_utils::VcEkfStatus::AHRS, app::can_tx::VC_EkfSolutionMode_get());
    ASSERT_FLOAT_EQ(0.4f, app::can_tx::VC_EulerAnglesRoll_get());
    ASSERT_FLOAT_EQ(-0.5f, app::can_tx::VC_EulerAnglesPitch_get());
    ASSERT_FLOAT_EQ(1.6f, app::can_tx::VC_EulerAnglesYaw_get());
}

TEST_F(VCSbgEllipsesTest, test_body_velocity)
{
    fakes::io::sbgEllipse::setVelocity(0u, 1.0f, 2.0f, 3.0f, 0.0f, 0.0f, 0.0f, 12.5f, -0.75f, 0.05f, 0.0f, 0.0f, 0.0f);

    ASSERT_FLOAT_EQ(12.5f, app::sbgEllipse::bodyVelX());
    ASSERT_FLOAT_EQ(-0.75f, app::sbgEllipse::bodyVelY());
    ASSERT_FLOAT_EQ(0.05f, app::sbgEllipse::bodyVelZ());
}

TEST_F(VCSbgEllipsesTest, test_global_velocity)
{
    fakes::io::sbgEllipse::setVelocity(0u, 8.0f, -6.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 3.0f, 0.0f, 0.0f, 0.0f);

    ASSERT_FLOAT_EQ(8.0f, app::sbgEllipse::globalVelN());
    ASSERT_FLOAT_EQ(-6.0f, app::sbgEllipse::globalVelE());
    ASSERT_FLOAT_EQ(0.5f, app::sbgEllipse::globalVelD());
}
