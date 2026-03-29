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
    checkSoftwareBspd(0.0f, app::bspdWarning::apps_brake_disagreement_state);
    ASSERT_TRUE(app::can_tx::VC_Warning_SoftwareBspd_get());
}
// brake disagreement warning inactive test
TEST_F(VCbspdTest, bspd_warning_inactive)
{
    checkSoftwareBspd(0.0f, app::bspdWarning::apps_brake_disagreement_state);
    ASSERT_FALSE(app::can_tx::VC_Warning_SoftwareBspd_get());
}
