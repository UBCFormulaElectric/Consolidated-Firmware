#include <gtest/gtest.h>

#include "ecuTestBase.hpp"
#include "app_tractiveSystem.hpp"
#include "test_fakes.hpp"
#include "app_canRx.hpp"
#include "app_canTx.hpp"

class AppTractiveSystemTest : public EcuTestBase
{
    protected:
    void board_setup() override
    {
        fakes::ts::reset();
        fakes::time::setTime(0);
        app::ts::broadcast();
    }
    void board_teardown() override {}

    // Sets both sensors to the same float value so the selected current is unambigious
    void setCurrent(float amps) 
    {
        fakes::ts::setCurrentLowResolution(amps);
        fakes::ts::setCurrentHighResolution(amps);
    }
};
// =============================================================================
// getVoltage() tests
// =============================================================================

TEST_F(AppTractiveSystemTest, getVoltage_returns_voltage_from_io) 
{
    fakes::ts::setVoltage(400.0f);
    EXPECT_FLOAT_EQ(app::ts::getVoltage(), 400.0f);
}

// =============================================================================
// getCurrent() tests
// =============================================================================

TEST_F(AppTractiveSystemTest, getCurrent_returns_high_res_current_when_low_res_within_range)
{
    fakes::ts::setCurrentLowResolution(50.0f);
    fakes::ts::setCurrentHighResolution(49.0f);
    // Should return the high res value because the low res value is within the ±70 A range
    EXPECT_FLOAT_EQ(app::ts::getCurrent(), 49.0f);
}

TEST_F(AppTractiveSystemTest, getCurrent_returns_low_res_current_when_low_res_out_of_range)
{
    fakes::ts::setCurrentLowResolution(100.0f);
    fakes::ts::setCurrentHighResolution(49.0f);
    // Should return the low res value because it is out of the ±70 A range
    EXPECT_FLOAT_EQ(app::ts::getCurrent(), 100.0f);
}

// =============================================================================
// broadcast() tests
// =============================================================================

TEST_F(AppTractiveSystemTest, broadcast_writes_voltage_current_power)
{
    fakes::ts::setVoltage(400.0f);
    setCurrent(50.0f);
    app::ts::broadcast();

    EXPECT_FLOAT_EQ(app::can_tx::BMS_TractiveSystemVoltage_get(), 400.0f);
    EXPECT_FLOAT_EQ(app::can_tx::BMS_TractiveSystemCurrent_get(), 50.0f);
    EXPECT_FLOAT_EQ(app::can_tx::BMS_TractiveSystemPower_get(), 20.0f); // 400 * 50 = 20000 W = 20 kW
}