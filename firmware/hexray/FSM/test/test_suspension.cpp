#include "fsmMocks.hpp"
#include "test_FSMBase.hpp"

extern "C" {
    #include "app_canTx.h"
    #include "app_canAlerts.h"
}

class SuspensionModuleTest : public FSMBaseTest{
};

TEST_F(SuspensionModuleTest, Normal_SuspensionConditions)
{
    fakes::io::suspension::setLeftTravel(1.0f);
    fakes::io::suspension::setRightTravel(2.0f);
    fakes::io::suspension::setLeftSensorOCSC(false);
    fakes::io::suspension::setRightSensorOCSC(false);

    LetTimePass(100);

    EXPECT_EQ(app_canTx_FSM_LeftSuspensionTravel_get(), 1.0f);
    EXPECT_EQ(app_canTx_FSM_RightSuspensionTravel_get(), 2.0f);
    EXPECT_FALSE(app_canAlerts_FSM_Info_LeftSuspensionOCSC_get());
    EXPECT_FALSE(app_canAlerts_FSM_Info_RightSuspensionOCSC_get());
}

TEST_F(SuspensionModuleTest, TravelOutOfRange_CANOverFlow)
{
    fakes::io::suspension::setLeftTravel(60.0f);    // Positive Travel, LARGER than CAN threshold psi(50)
    fakes::io::suspension::setRightTravel(60.0f);
    fakes::io::suspension::setLeftSensorOCSC(false);
    fakes::io::suspension::setRightSensorOCSC(false);

    LetTimePass(100);

    EXPECT_EQ(app_canTx_FSM_LeftSuspensionTravel_get(), 50.0f);
    EXPECT_EQ(app_canTx_FSM_RightSuspensionTravel_get(), 50.0f);
    EXPECT_FALSE(app_canAlerts_FSM_Info_LeftSuspensionOCSC_get());
    EXPECT_FALSE(app_canAlerts_FSM_Info_RightSuspensionOCSC_get());
}

TEST_F(SuspensionModuleTest, TravelOutOfRange_CANUnderFlow)
{
    fakes::io::suspension::setLeftTravel(-10.0f);   // Negative Travel, SMALLER than CAN threshold (0)
    fakes::io::suspension::setRightTravel(-10.0f);
    fakes::io::suspension::setLeftSensorOCSC(false);
    fakes::io::suspension::setRightSensorOCSC(false);

    LetTimePass(100);

    EXPECT_EQ(app_canTx_FSM_LeftSuspensionTravel_get(), 0.0f);
    EXPECT_EQ(app_canTx_FSM_RightSuspensionTravel_get(), 0.0f);
    EXPECT_FALSE(app_canAlerts_FSM_Info_LeftSuspensionOCSC_get());
    EXPECT_FALSE(app_canAlerts_FSM_Info_RightSuspensionOCSC_get());
}


TEST_F(SuspensionModuleTest, OCSC_Left)
{
    fakes::io::suspension::setLeftTravel(1.0f);
    fakes::io::suspension::setRightTravel(2.0f);
    fakes::io::suspension::setLeftSensorOCSC(true);
    fakes::io::suspension::setRightSensorOCSC(false);

    LetTimePass(100);

    EXPECT_EQ(app_canTx_FSM_LeftSuspensionTravel_get(), 1.0f);
    EXPECT_EQ(app_canTx_FSM_RightSuspensionTravel_get(), 2.0f);
    EXPECT_TRUE(app_canAlerts_FSM_Info_LeftSuspensionOCSC_get());
    EXPECT_FALSE(app_canAlerts_FSM_Info_RightSuspensionOCSC_get());
}

TEST_F(SuspensionModuleTest, OCSC_Right)
{
    fakes::io::suspension::setLeftTravel(1.0f);
    fakes::io::suspension::setRightTravel(2.0f);
    fakes::io::suspension::setLeftSensorOCSC(false);
    fakes::io::suspension::setRightSensorOCSC(true);

    LetTimePass(100);

    EXPECT_EQ(app_canTx_FSM_LeftSuspensionTravel_get(), 1.0f);
    EXPECT_EQ(app_canTx_FSM_RightSuspensionTravel_get(), 2.0f);
    EXPECT_FALSE(app_canAlerts_FSM_Info_LeftSuspensionOCSC_get());
    EXPECT_TRUE(app_canAlerts_FSM_Info_RightSuspensionOCSC_get());
}