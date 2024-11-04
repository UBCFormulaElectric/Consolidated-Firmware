#include <gtest/gtest.h>
#include "test_bmsBaseStateMachineTest.h"

class BmsFaultTest : public BmsBaseStateMachineTest
{
};

TEST_F(BmsFaultTest, check_all_cell_voltages_segment0)
{
    // Test that every cell voltage in Segment 0 is being properly transmitted to CAN

    TearDown();
    SetUp();
    float fake_voltage = 3.0f;
    fake_io_ltc6813CellVoltages_getCellVoltage_returnsForAnyArgs(fake_voltage);
    LetTimePass(1000);
    app_diagnosticsMode_broadcast();
    ASSERT_EQ(fake_voltage, app_canTx_BMS_Seg0_Cell0_Voltage_get());
    ASSERT_EQ(fake_voltage, app_canTx_BMS_Seg0_Cell1_Voltage_get());
}
