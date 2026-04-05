#include <gtest/gtest.h>
#include "test/test_VCBase.hpp"
#include "app_bspdwarning.hpp"
#include "vc_fakes.hpp"
#include "util_errorCodes.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canTx.h" // idk why theres a hezray FSM 
class VCbspdTest : public VCBaseTest
{
    const app::Signal apps_brake_disagreement_state_test{ 10u,
                                                 10u };
};

// brake disagreement warning active test
TEST_F(VCbspdTest, bspd_warning_active)
{
    app_canTx_FSM_BrakeActuated_set(true);
    LetTimePass(1);
    app::bspdWarning::checkSoftwareBspd(0.50f, apps_brake_disagreement_state_test);
    LetTimePass(1);
    ASSERT_TRUE(app::can_tx::VC_Warning_SoftwareBspd_get());
}

// brake disagreement warning inactive test
TEST_F(VCbspdTest, bspd_warning_inactive)
{
    app_canTx_FSM_BrakeActuated_set(false);
    LetTimePass(1);
    app::bspdWarning::checkSoftwareBspd(0.50f, apps_brake_disagreement_state_test);
    LetTimePass(1);
    ASSERT_FALSE(app::can_tx::VC_Warning_SoftwareBspd_get());
}

TEST_F(VCbspdTest, bspd_warning_inactive_no_brake_actuation)
{
    app_canTx_FSM_BrakeActuated_set(true);
    LetTimePass(1);
    app::bspdWarning::checkSoftwareBspd(0.00f, apps_brake_disagreement_state_test);
    LetTimePass(1);
    ASSERT_FALSE(app::can_tx::VC_Warning_SoftwareBspd_get());
}
