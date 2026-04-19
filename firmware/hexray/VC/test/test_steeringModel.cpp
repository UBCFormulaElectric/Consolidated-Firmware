#include <gtest/gtest.h>
#include "test/test_VCBase.hpp"

#include "vc_fakes.hpp"
#include "torque_vectoring/estimation/steering_model.hpp"
#include "torque_vectoring/datatypes/datatypes_vd_constants.hpp"

class TVSteeringModelTest : public VCBaseTest
{
};

using namespace app::tv::estimators;
using namespace app::tv::datatypes;

TEST_F(TVSteeringModelTest, ZeroSteeringAngle)
{
    steering::WheelSteerAngles ws_ang_rad = steering::wheel_steer_angles(0.0f);

    ASSERT_FLOAT_EQ(ws_ang_rad.rr_rad, 0.0f);
    ASSERT_FLOAT_EQ(ws_ang_rad.rl_rad, 0.0f);
    ASSERT_FLOAT_EQ(ws_ang_rad.fr_rad, 0.0f);
    ASSERT_FLOAT_EQ(ws_ang_rad.fl_rad, 0.0f);
}

TEST_F(TVSteeringModelTest, MaxSteeringAngleRightTurn)
{
    steering::WheelSteerAngles ws_ang_rad = steering::wheel_steer_angles(vd_constants::STEER_WHEEL_RANGE_rad);

    ASSERT_FLOAT_EQ(ws_ang_rad.rr_rad, 0.0f);
    ASSERT_FLOAT_EQ(ws_ang_rad.rl_rad, 0.0f);
    EXPECT_NEAR(0.395840674f, ws_ang_rad.fr_rad, 0.002f);
    EXPECT_NEAR(0.414166631f, ws_ang_rad.fl_rad, 0.0065f);
}

TEST_F(TVSteeringModelTest, MaxSteeringAngleLeftTurn)
{
    steering::WheelSteerAngles ws_ang_rad = steering::wheel_steer_angles(-vd_constants::STEER_WHEEL_RANGE_rad);

    ASSERT_FLOAT_EQ(ws_ang_rad.rr_rad, 0.0f);
    ASSERT_FLOAT_EQ(ws_ang_rad.rl_rad, 0.0f);
    EXPECT_NEAR(-0.414166631f, ws_ang_rad.fr_rad, 0.0065f);
    EXPECT_NEAR(-0.395840674f, ws_ang_rad.fl_rad, 0.002f);
}
