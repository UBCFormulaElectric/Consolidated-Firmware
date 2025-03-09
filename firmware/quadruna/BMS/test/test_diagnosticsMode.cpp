#include <gtest/gtest.h>
#include "test_bmsBaseStateMachineTest.h"

class BmsFaultTest : public BmsBaseStateMachineTest
{
};

TEST_F(BmsFaultTest, check_cell_voltage_segment0_cell0)
{
    /**
     * Test cell voltage in Segment 0 is being properly transmitted to CAN.
     */

    // Setting up the environment.
    float fake_voltage = 1.0f;
    fake_io_ltc6813CellVoltages_getCellVoltage_returnsForAnyArgs(fake_voltage);

    // Passing time to let the message transmit.
    LetTimePass(1000);

    // Passing time to let the message transmit.
    LetTimePass(1000);

    // Test assertion.
    ASSERT_EQ(fake_voltage, app_canTx_BMS_Seg0_Cell0_Voltage_get());
}

TEST_F(BmsFaultTest, check_temperature_segment0)
{
    /**
     * Test temperature in segment 0 is being properly transmitted to CAN.
     */

    // Setting up the environment.
    float fake_temperature = 2.0f;
    fake_io_ltc6813CellTemperatures_getSpecificCellTempDegC_returnsForAnyArgs(fake_temperature);

    // Passing time to let the message transmit.
    LetTimePass(1000);

    // Passing time to let the message transmit.
    LetTimePass(1000);

    // Test assertion.
    ASSERT_EQ(fake_temperature, app_canTx_BMS_Seg0_Temp_get());
}
