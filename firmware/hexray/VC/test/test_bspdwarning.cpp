#include <gtest/gtest.h>
#include "test/test_VCBase.hpp"
#include "app_bspdwarning.hpp"
#include "vc_fakes.hpp"
#include "util_errorCodes.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"

class VCbspdTest : public VCBaseTest
{
};

// brake disagreement warning active test
TEST_F(VCbspdTest, bspd_warning_active)
{
    //app::can_rx::FSM_BrakeActuated_get();
    fakes::io::bspdWarning::set_BrakeActuated_state(true);
    app::bspdWarning::checkSoftwareBspd(0.50f, app::bspdWarning::apps_brake_disagreement_state);
    ASSERT_TRUE(app::can_tx::VC_Warning_SoftwareBspd_get());
}

// brake disagreement warning inactive test
TEST_F(VCbspdTest, bspd_warning_inactive)
{
    fakes::io::bspdWarning::set_BrakeActuated_state(false);
    app::bspdWarning::checkSoftwareBspd(0.50f, app::bspdWarning::apps_brake_disagreement_state);
    ASSERT_FALSE(app::can_tx::VC_Warning_SoftwareBspd_get());
}

TEST_F(VCbspdTest, bspd_warning_inactive_no_brake_actuation)
{
    fakes::io::bspdWarning::set_BrakeActuated_state(true);
    app::bspdWarning::checkSoftwareBspd(0.00f, app::bspdWarning::apps_brake_disagreement_state);
    ASSERT_FALSE(app::can_tx::VC_Warning_SoftwareBspd_get());
}
