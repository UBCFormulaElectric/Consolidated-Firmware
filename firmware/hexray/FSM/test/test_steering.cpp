#include "fsmMocks.hpp"
#include "test_FSMBase.hpp"
extern "C" {
#include "app_canTx.h"
#include "app_canAlerts.h"
}

class SteeringModuleTest : public FSMBaseTest{
};

TEST_F(SteeringModuleTest, normal_SteeringConditions)
{
    fakes::io::steering::setAngleDegrees(15.0f);
    fakes::io::steering::setOCSC(false);

    LetTimePass(100);

    EXPECT_FLOAT_EQ(app_canTx_FSM_SteeringAngle_get(), 15.0f);
    EXPECT_FALSE(app_canAlerts_FSM_Info_SteeringAngleOCSC_get());
    EXPECT_FALSE(app_canTx_FSM_Info_SteeringAngleOutOfRange_get());
}