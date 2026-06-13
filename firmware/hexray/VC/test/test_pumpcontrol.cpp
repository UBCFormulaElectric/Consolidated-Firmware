#include <gtest/gtest.h>

#include "app_pumpControl.hpp"
#include "app_canTx.hpp"
#include "io_pumps.hpp"
#include "io_pumpControllerFake.hpp"
#include "test/test_VCBase.hpp"

class VCPumpControlTest : public VCBaseTest
{
};

TEST_F(VCPumpControlTest, reset_clears_pump_outputs)
{
    app::pumpControl::reset();

    ASSERT_FLOAT_EQ(0.0f, app::can_tx::VC_RLPumpSetpoint_get());
    ASSERT_FLOAT_EQ(0.0f, app::can_tx::VC_RRPumpSetpoint_get());
    ASSERT_TRUE(rr_pump.getPercentage().has_value());
    ASSERT_EQ(0u, rr_pump.getPercentage().value());
}

TEST_F(VCPumpControlTest, restart_stops_rr_pump_and_zeroes_setpoints)
{
    ASSERT_TRUE(rr_pump.setPercentage(37).has_value());

    app::pumpControl::restart();

    ASSERT_FLOAT_EQ(0.0f, app::can_tx::VC_RLPumpSetpoint_get());
    ASSERT_FLOAT_EQ(0.0f, app::can_tx::VC_RRPumpSetpoint_get());
    ASSERT_TRUE(rr_pump.getPercentage().has_value());
    ASSERT_EQ(0u, rr_pump.getPercentage().value());
}

TEST_F(VCPumpControlTest, monitor_pumps_leaves_outputs_zero_when_disabled)
{
    app::pumpControl::reset();
    fakes::io::pumpController::set_pumps_ok(false);
    fakes::io::pumpController::set_pumps_enable(false);

    app::pumpControl::MonitorPumps();

    ASSERT_FLOAT_EQ(0.0f, app::can_tx::VC_RLPumpSetpoint_get());
    ASSERT_FLOAT_EQ(0.0f, app::can_tx::VC_RRPumpSetpoint_get());
    ASSERT_TRUE(rr_pump.getPercentage().has_value());
    ASSERT_EQ(0u, rr_pump.getPercentage().value());
}