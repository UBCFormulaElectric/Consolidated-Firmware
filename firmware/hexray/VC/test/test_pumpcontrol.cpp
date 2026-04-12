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

TEST_F(VCPumpControlTest, pump_failure_when_pumps_not_ready)
{
    app::pumpControl::reset();
    io::pumpController::pumps_ok(false);
    io::pumpController::pumps_enabled(true);
    app::pumpControl::MonitorPumps();
    ASSERT_TRUE(app::can_tx::VC_PumpFailure_get());
    ASSERT_FALSE(app::can_tx::VC_RsmTurnOnPump_get());
}

TEST_F(VCPumpControlTest, no_failure_when_pumps_ready)
{
    app::pumpControl::reset();
    io::pumpController::pumps_ok(true);
    io::pumpController::pumps_enabled(true);
    app::pumpControl::MonitorPumps();
    ASSERT_FALSE(app::can_tx::VC_PumpFailure_get());
    ASSERT_TRUE(app::can_tx::VC_RsmTurnOnPump_get());
}

TEST_F(VCPumpControlTest, rampup_setpoint_starts_at_five_percent)
{
    app::pumpControl::reset();
    io::pumpController::pumps_ok(true);
    io::pumpController::pumps_enabled(true);
    app::pumpControl::MonitorPumps();
    ASSERT_FLOAT_EQ(5.0f, app::can_tx::VC_PumpRampUpSetPoint_get());
}

TEST_F(VCPumpControlTest, rampup_completes_without_failure)
{
    app::pumpControl::reset();
    io::pumpController::pumps_ok(true);
    io::pumpController::pumps_enabled(true);
    for (int i = 0; i < 19; ++i)
    {
        app::pumpControl::MonitorPumps();
    }
    ASSERT_FALSE(app::can_tx::VC_PumpFailure_get());
    ASSERT_TRUE(app::can_tx::VC_RsmTurnOnPump_get());
}

TEST_F(VCPumpControlTest, pump_failure_resets_rampup)
{
    app::pumpControl::reset();
    io::pumpController::pumps_ok(true);
    io::pumpController::pumps_enabled(true);
    app::pumpControl::MonitorPumps();

    io::pumpController::pumps_ok(false);
    app::pumpControl::MonitorPumps();

    ASSERT_TRUE(app::can_tx::VC_PumpFailure_get());
}
