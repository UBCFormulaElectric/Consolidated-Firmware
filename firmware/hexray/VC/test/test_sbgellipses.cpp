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

// Testing broadcating
TEST_F(VCSbgEllipsesTest, test_broadcast) {
    app::sbgEllipse::broadcast();
    ASSERT_FLOAT_EQ(0.0f, app::can_tx::VC_EkfRoll_get());
    ASSERT_FLOAT_EQ(0.0f, app::can_tx::VC_EkfPitch_get());
    ASSERT_FLOAT_EQ(0.0f, app::can_tx::VC_EkfYaw_get());
}

// Testing getting vehicle velocity
TEST_F(VCSbgEllipsesTest, test_get_vehicle_velocity) {
        io::sbgEllipse::VelocityData velocityData{};
        const auto result = app::sbgEllipse::getVehicleVelocity(&velocityData);
        ASSERT_FLOAT_EQ(0.0f, velocityData.north);
        ASSERT_FLOAT_EQ(0.0f, velocityData.east);
        ASSERT_FLOAT_EQ(0.0f, velocityData.down);
        ASSERT_FLOAT_EQ(0.0f, velocityData.roll);
        ASSERT_FLOAT_EQ(0.0f, velocityData.pitch);
        ASSERT_FLOAT_EQ(0.0f, velocityData.yaw);
}

// Testing getting ekf solution mode
TEST_F(VCSbgEllipsesTest, test_get_ekf_solution_mode) {
    const auto ekfSolutionMode = app::
    sbgEllipse::getEkfSolutionMode();
    ASSERT_EQ(0u, ekfSolutionMode);
}
