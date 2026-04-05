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
    fakes::io::rl_pump::pumps_ok(false);
    app::rl_pump.update();
    app::pumpControl::MonitorPumps();
    ASSERT_FALSE(app::can_tx::VC_PumpFailure_get());
}

// Testing when rr pumps not ready
TEST_F(VCPumpControlTest, test_rr_pump_not_ready) {
    fakes::io::rr_pump::pumps_ok(true);
        app::pumpControl::MonitorPumps();
    ASSERT_FALSE(app::can_tx::VC_PumpFailure_get());
}

// Testing when rampup percentage is at 50%
TEST_F(VCPumpControlTest, test_rampup_percentage_50) {
    fakes::io::rr_pump::pumps_ok(false);
        app::pumpControl::MonitorPumps();
    ASSERT_TRUE(app::can_tx::VC_PumpFailure_get());
}

// Testing when rampup percentage is at 95%
TEST_F(VCPumpControlTest, test_rampup_percentage_95) {
    fakes::io::rr_pump::rampUpSetPoint(0.95f);
        app::pumpControl::MonitorPumps();
    ASSERT_FALSE(app::can_tx::VC_PumpFailure_get());
}

// Testing when pumpFailure is true
TEST_F(VCPumpControlTest, test_pumpFailure_true) {
    fakes::io::rr_pump::pumps_ok(false);
    LetTimePass(1);
    app::pumpControl::MonitorPumps();
    ASSERT_TRUE(app::can_tx::VC_PumpFailure_get());
}
