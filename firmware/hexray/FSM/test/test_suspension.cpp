#include "fsmMocks.hpp"
#include "test_FSMBase.hpp"

extern "C" {
    #include "app_canTx.h"
    #include "app_canAlerts.h"
}

class SuspensionModuleTest : public FSMBaseTest{
};

TEST_F(SuspensionModuleTest, normal_suspensionConditions)
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