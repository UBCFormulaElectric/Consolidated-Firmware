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
    io::sbgEllipse::setGeneralStatus(0x1234u);
    io::sbgEllipse::setComStatus(0x56789ABCu);
    io::sbgEllipse::setOverflowCount(7u);
    io::sbgEllipse::setTimestampUs(424242u);
    io::sbgEllipse::setVelocity(9u, 1.25f, -2.5f, 3.75f, 0.1f, 0.2f, 0.3f);
    io::sbgEllipse::setSolutionMode(2u);
    io::sbgEllipse::setAttitude(0.4f, -0.5f, 1.6f);

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
    ASSERT_EQ(static_cast<app::can_utils::VcEkfStatus>(2), app::can_tx::VC_EkfSolutionMode_get());
    ASSERT_FLOAT_EQ(0.4f, app::can_tx::VC_EulerAnglesRoll_get());
    ASSERT_FLOAT_EQ(-0.5f, app::can_tx::VC_EulerAnglesPitch_get());
    ASSERT_FLOAT_EQ(1.6f, app::can_tx::VC_EulerAnglesYaw_get());
}

TEST_F(VCSbgEllipsesTest, test_get_vehicle_velocity)
{
    io::sbgEllipse::VelocityData velocityData{ 0u, 3.0f, 4.0f, 12.0f, 0.0f, 0.0f, 0.0f };
    ASSERT_FLOAT_EQ(13.0f, app::sbgEllipse::getVehicleVelocity(velocityData));
}

TEST_F(VCSbgEllipsesTest, test_get_ekf_solution_mode)
{
    io::sbgEllipse::setSolutionMode(3u);
    app::sbgEllipse::broadcast();
    ASSERT_EQ(static_cast<app::can_utils::VcEkfStatus>(3), app::sbgEllipse::getEkfSolutionMode());
}
