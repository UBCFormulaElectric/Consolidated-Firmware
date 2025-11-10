#include "fsmMocks.hpp"
#include "test_FSMBase.hpp"

extern "C" {
    #include "app_canTx.h"
    #include "app_canAlerts.h"
}

// TEST NOT COMPLETED: 
// Test values are not accurate, App logic is not yet finalized

class AppsModuleTest : public FSMBaseTest{
};

TEST_F(AppsModuleTest, normal_appConditions) {
    fakes::io::apps::setPrimaryPressure(30.0f);
    fakes::io::apps::setPrimaryOCSC(false);
    fakes::io::apps::setSecondaryPressure(25.0f);
    fakes::io::apps::setSecondaryOCSC(false);

    LetTimePass(100);

    EXPECT_FALSE(app_canAlerts_FSM_Warning_PappsOCSC_get());
    EXPECT_FALSE(app_canAlerts_FSM_Warning_SappsOCSC_get());
    EXPECT_FALSE(app_canAlerts_FSM_Warning_AppsDisagreement_get());

    EXPECT_EQ(app_canTx_FSM_PappsRawPedalPercentage_get(), 30.0f);
    EXPECT_EQ(app_canTx_FSM_SappsRawPedalPercentage_get(), 25.0f);
    EXPECT_EQ(app_canTx_FSM_PappsMappedPedalPercentage_get(), 30.0f);
    EXPECT_EQ(app_canTx_FSM_SappsMappedPedalPercentage_get(), 25.0f);
}