/*#include "fsmMocks.hpp"
#include "test_FSMBase.hpp"
extern "C" {
    #include "app_canTx.h"
    #include "app_canAlerts.h"
}

class ShdnLoopModuleTest : public FSMBaseTest{
};

TEST_F(ShdnLoopModuleTest, normal_ShutdownLoopConditions) {
    fakes::io::shdnLoop::setTsmsFault(false);
    fakes::io::shdnLoop::setInertiaSwitch(false);
    fakes::io::shdnLoop::setRearRightMotorInterlock(false);
    fakes::io::shdnLoop::setSplitterBoxInterlockFault(false);

    LetTimePass(100);

    EXPECT_FALSE(app_canTx_FSM_ShutdownLoop_TSMSFault_get());
    EXPECT_FALSE(app_canTx_FSM_ShutdownLoop_InertiaSwitch_get());
    EXPECT_FALSE(app_canTx_FSM_ShutdownLoop_RearRightMotorInterlock_get());
    EXPECT_FALSE(app_canTx_FSM_ShutdownLoop_SplitterBoxInterlockFault_get());
}*/