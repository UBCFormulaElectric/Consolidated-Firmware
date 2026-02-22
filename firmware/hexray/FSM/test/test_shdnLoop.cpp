// TODO
#include "fsmMocks.hpp"
#include "test_FSMBase.hpp"
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"

class ShdnLoopModuleTest : public FSMBaseTest
{
};

/*

*******SKIP ShdnLoop Testing for now, WAIT HW/IO Setup*******************

TEST_F(ShdnLoopModuleTest, normal_ShutdownLoopConditions) {
    // All shutdowns are OK
    fakes::io::fsmShdn::setCockpitShdn(true);
    fakes::io::fsmShdn::setBotsShdn(true);
    fakes::io::fsmShdn::setFLShdn(true);
    fakes::io::fsmShdn::setFRShdn(true);

    LetTimePass(100);

    EXPECT_TRUE(app_canTx_FSM_COCKPITOKStatus_get());
    EXPECT_TRUE(app_canTx_FSM_BOTSOKStatus_get());
    EXPECT_TRUE(app_canTx_FSM_FrontLeftILCKInertiaOKStatus_get());
    EXPECT_TRUE(app_canTx_FSM_FrontRightILCKOKStatus_get());
}*/