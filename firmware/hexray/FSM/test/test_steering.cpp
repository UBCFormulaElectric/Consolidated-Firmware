#include "fsmMocks.hpp"
#include "test_FSMBase.hpp"
extern "C"
{
#include "app_canTx.h"
#include "app_canAlerts.h"
}

class SteeringModuleTest : public FSMBaseTest
{
};

TEST_F(SteeringModuleTest, Normal_SteeringConditions_1)
{
    fakes::io::steering::setAngleDegrees(15.0f);
    fakes::io::steering::setOCSC(false);

    LetTimePass(100);

    EXPECT_FLOAT_EQ(app_canTx_FSM_SteeringAngle_get(), 15.0f);
    EXPECT_FALSE(app_canAlerts_FSM_Info_SteeringAngleOCSC_get());
    EXPECT_FALSE(app_canTx_FSM_Info_SteeringAngleOutOfRange_get());
}

TEST_F(SteeringModuleTest, Normal_SteeringConditions_2)
{
    fakes::io::steering::setAngleDegrees(95.0f); // Edge Case: Postive MAX Angle
    fakes::io::steering::setOCSC(false);

    LetTimePass(100);

    EXPECT_FLOAT_EQ(app_canTx_FSM_SteeringAngle_get(), 95.0f);
    EXPECT_FALSE(app_canAlerts_FSM_Info_SteeringAngleOCSC_get());
    EXPECT_FALSE(app_canTx_FSM_Info_SteeringAngleOutOfRange_get());
}

TEST_F(SteeringModuleTest, Normal_SteeringConditions_3)
{
    fakes::io::steering::setAngleDegrees(-95.0f); // Edge Case: Negative MAX Angle
    fakes::io::steering::setOCSC(false);

    LetTimePass(100);

    EXPECT_FLOAT_EQ(app_canTx_FSM_SteeringAngle_get(), -95.0f);
    EXPECT_FALSE(app_canAlerts_FSM_Info_SteeringAngleOCSC_get());
    EXPECT_FALSE(app_canTx_FSM_Info_SteeringAngleOutOfRange_get());
}

TEST_F(SteeringModuleTest, AngleOutOfRangle_Overflow)
{
    fakes::io::steering::setAngleDegrees(100.0f); // Within CAN Threshold
    fakes::io::steering::setOCSC(false);

    LetTimePass(100);

    EXPECT_FLOAT_EQ(app_canTx_FSM_SteeringAngle_get(), 100.0f);
    EXPECT_FALSE(app_canAlerts_FSM_Info_SteeringAngleOCSC_get());
    EXPECT_TRUE(app_canTx_FSM_Info_SteeringAngleOutOfRange_get());
}

TEST_F(SteeringModuleTest, AngleOutOfRangle_CANOverflow)
{
    fakes::io::steering::setAngleDegrees(120.0f); // Over CAN Threshold (110)
    fakes::io::steering::setOCSC(false);

    LetTimePass(100);

    EXPECT_FLOAT_EQ(app_canTx_FSM_SteeringAngle_get(), 110.0f); // Clamped Value
    EXPECT_FALSE(app_canAlerts_FSM_Info_SteeringAngleOCSC_get());
    EXPECT_TRUE(app_canTx_FSM_Info_SteeringAngleOutOfRange_get());
}

TEST_F(SteeringModuleTest, AngleOutOfRangle_Underflow)
{
    fakes::io::steering::setAngleDegrees(-100.0f); // Within CAN Threshold
    fakes::io::steering::setOCSC(false);

    LetTimePass(100);

    EXPECT_FLOAT_EQ(app_canTx_FSM_SteeringAngle_get(), -100.0f);
    EXPECT_FALSE(app_canAlerts_FSM_Info_SteeringAngleOCSC_get());
    EXPECT_TRUE(app_canTx_FSM_Info_SteeringAngleOutOfRange_get());
}

TEST_F(SteeringModuleTest, AngleOutOfRangle_CANUnderflow)
{
    fakes::io::steering::setAngleDegrees(-120.0f); // Over CAN Threshold (-110)
    fakes::io::steering::setOCSC(false);

    LetTimePass(100);

    EXPECT_FLOAT_EQ(app_canTx_FSM_SteeringAngle_get(), -110.0f); // Clamped Value
    EXPECT_FALSE(app_canAlerts_FSM_Info_SteeringAngleOCSC_get());
    EXPECT_TRUE(app_canTx_FSM_Info_SteeringAngleOutOfRange_get());
}

TEST_F(SteeringModuleTest, OCSC)
{
    fakes::io::steering::setAngleDegrees(15.0f);
    fakes::io::steering::setOCSC(true); // Opened or Shorted Circuit

    LetTimePass(100);

    EXPECT_FLOAT_EQ(app_canTx_FSM_SteeringAngle_get(), 15.0f);
    EXPECT_TRUE(app_canAlerts_FSM_Info_SteeringAngleOCSC_get());
    EXPECT_FALSE(app_canTx_FSM_Info_SteeringAngleOutOfRange_get());
}

TEST_F(SteeringModuleTest, Multiple_Faults)
{
    fakes::io::steering::setAngleDegrees(120.0f);
    fakes::io::steering::setOCSC(true); // Opened or Shorted Circuit

    LetTimePass(100);

    EXPECT_FLOAT_EQ(app_canTx_FSM_SteeringAngle_get(), 110.0f);
    EXPECT_TRUE(app_canAlerts_FSM_Info_SteeringAngleOCSC_get());
    EXPECT_TRUE(app_canTx_FSM_Info_SteeringAngleOutOfRange_get());
}
