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
TEST_F(VCPumpControlTest, test_rl_pump_not_ready) {}

// Testing when rr pumps not ready
TEST_F(VCPumpControlTest, test_rr_pump_not_ready) {}

// Testing when rampup percentage is at 50%
TEST_F(VCPumpControlTest, test_rampup_percentage_50) {}

// Testing when rampup percentage is at 95%
TEST_F(VCPumpControlTest, test_rampup_percentage_95) {}

// Testing when pumpFailure is true
TEST_F(VCPumpControlTest, test_pumpFailure_true) {}
