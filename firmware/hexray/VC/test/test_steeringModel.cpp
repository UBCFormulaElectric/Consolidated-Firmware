#include <gtest/gtest.h>
#include "test/test_VCBase.hpp"

#include "vc_fakes.hpp"
#include "torque_vectoring/estimation/steering_model.hpp"
#include "torque_vectoring/shared_datatypes/constants.hpp"
#include "util_utils.hpp"

class TVSteeringModelTest : public VCBaseTest
{
};

using namespace app::tv::estimators;
using namespace app::tv::shared_datatypes;

TEST_F(TVSteeringModelTest, ZeroSteeringAngle)
{
    wheel_set<float> ws_ang_rad = steering::wheel_steer_angles(0.0f);

    ASSERT_FLOAT_EQ(ws_ang_rad.rr, 0.0f);
    ASSERT_FLOAT_EQ(ws_ang_rad.rl, 0.0f);
    ASSERT_FLOAT_EQ(ws_ang_rad.fr, 0.0f);
    ASSERT_FLOAT_EQ(ws_ang_rad.fl, 0.0f);
}

TEST_F(TVSteeringModelTest, MaxSteeringAngleRightTurn)
{
    wheel_set<float> ws_ang_rad = steering::wheel_steer_angles(vd_constants::STEER_WHEEL_RANGE_rad);

    ASSERT_FLOAT_EQ(ws_ang_rad.rr, 0.0f);
    ASSERT_FLOAT_EQ(ws_ang_rad.rl, 0.0f);
    EXPECT_NEAR(0.395840674f, ws_ang_rad.fr, 0.002f);
    EXPECT_NEAR(0.414166631f, ws_ang_rad.fl, 0.0065f);
}

TEST_F(TVSteeringModelTest, MaxSteeringAngleLeftTurn)
{
    wheel_set<float> ws_ang_rad = steering::wheel_steer_angles(-vd_constants::STEER_WHEEL_RANGE_rad);

    ASSERT_FLOAT_EQ(ws_ang_rad.rr, 0.0f);
    ASSERT_FLOAT_EQ(ws_ang_rad.rl, 0.0f);
    EXPECT_NEAR(-0.414166631f, ws_ang_rad.fr, 0.0065f);
    EXPECT_NEAR(-0.395840674f, ws_ang_rad.fl, 0.002f);
}
