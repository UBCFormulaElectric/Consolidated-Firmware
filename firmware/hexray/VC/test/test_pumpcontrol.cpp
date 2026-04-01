#include <gtest/gtest.h>
#include "test/test_VCBase.hpp"
#include "app_pumpControl.hpp"
#include "vc_fakes.hpp"
#include "util_errorCodes.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"

class VCPumpControlTest : public VCBaseTest
{
};

// Testing when rl pumps not ready
TEST_F(VCPumpControlTest, test_rl_pump_not_ready) {
    rl_pump.pumps_ok(false);
    app::rl_pump.update();
    ASSERT_FALSE(app::can_tx::VC_PumpsReady_get());
}

// Testing when rr pumps not ready
TEST_F(VCPumpControlTest, test_rr_pump_not_ready) {
    rr_pump.pumps_ok(false);
    app::rr_pump.update();
    ASSERT_FALSE(app::can_tx::VC_PumpsReady_get());
}

// Testing when rampup percentage is at 50%
TEST_F(VCPumpControlTest, test_rampup_percentage_50) {
    rr_pump.pumps_ok(false);
    rr_pump.update();
    ASSERT_FALSE(app::can_tx::VC_PumpsReady_get());
}

// Testing when rampup percentage is at 95%
TEST_F(VCPumpControlTest, test_rampup_percentage_95) {
    rr_pump::rampUpPercentage(0.95f);
    app::rampUp();
    ASSERT_FALSE(app::can_tx::VC_PumpsReady_get());
}

// Testing when pumpFailure is true
TEST_F(VCPumpControlTest, test_pumpFailure_true) {
    rr_pump::pumps_ok(false);
    app::rr_pump::update();
    ASSERT_TRUE(app::can_tx::VC_PumpFailure_get());
}
