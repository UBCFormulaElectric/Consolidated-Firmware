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
    // takes at least 10 ms for the signal to activate
    for (int i = 0; i < 11; i++)
    {
        app::can_rx::FSM_BrakeActuated_update(true);
        app::bspdWarning::checkSoftwareBspd(0.50f);
        LetTimePass(1);
    }
    ASSERT_TRUE(app::can_tx::VC_Warning_SoftwareBspd_get());
    app::bspdWarning::reset();
}

// brake disagreement warning inactive test
TEST_F(VCbspdTest, bspd_warning_inactive)
{
    app::can_rx::FSM_BrakeActuated_update(false);
    app::bspdWarning::checkSoftwareBspd(0.50f);
    LetTimePass(1);
    ASSERT_FALSE(app::can_tx::VC_Warning_SoftwareBspd_get());
    app::bspdWarning::reset();
}

TEST_F(VCbspdTest, bspd_warning_inactive_no_brake_actuation)
{
    app::can_rx::FSM_BrakeActuated_update(true);
    app::bspdWarning::checkSoftwareBspd(0.00f);
    LetTimePass(1);
    ASSERT_FALSE(app::can_tx::VC_Warning_SoftwareBspd_get());
    app::bspdWarning::reset();
}

TEST_F(VCbspdTest, bspd_warning_inactive_no_brake_no_papps)
{
    app::can_rx::FSM_BrakeActuated_update(false);
    app::bspdWarning::checkSoftwareBspd(0.00f);
    LetTimePass(1);
    ASSERT_FALSE(app::can_tx::VC_Warning_SoftwareBspd_get());
    app::bspdWarning::reset();
}
