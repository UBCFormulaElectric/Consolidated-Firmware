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

    // Reciving and transmitting the data.
    app_canRx_Debug_BMSDiagnosticsRequest_update(true);
    app_canRx_Debug_SegmentCellVoltageRequest_update(SEG_0);

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

    // Reciving and transmitting the data.
    app_canRx_Debug_BMSDiagnosticsRequest_update(true);
    app_canRx_Debug_SegmentTemperaturesRequest_update(true);

    // Passing time to let the message transmit.
    LetTimePass(1000);

    // Test assertion.
    ASSERT_EQ(fake_temperature, app_canTx_BMS_Seg0_Temp_get());
}

TEST_F(BmsFaultTest, check_diagnostics_mode_is_off_on)
{
    /**
     * Test that neither voltage nor temperature reading are being transmitted when diagnostics
     * mode is off (even if individual temperature/voltage reading modes are turned on).
     */

    // Setting up the environment.
    float fake_voltage = 3.0f;
    fake_io_ltc6813CellVoltages_getCellVoltage_returnsForAnyArgs(fake_voltage);

    // Passing time to let the message transmit.
    LetTimePass(1000);

    // Reciving and transmitting the data.
    app_canRx_Debug_BMSDiagnosticsRequest_update(false);
    app_canRx_Debug_SegmentCellVoltageRequest_update(SEG_0);

    // Passing time to let the message transmit.
    LetTimePass(1000);

    // Test assertion.
    ASSERT_EQ(0, app_canTx_BMS_Seg0_Cell0_Voltage_get());
}
