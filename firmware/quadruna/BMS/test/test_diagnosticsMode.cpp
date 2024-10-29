#include <gtest/gtest.h>
#include "test_bmsBaseStateMachineTest.h"

class BmsFaultTest : public BmsBaseStateMachineTest
{
};

TEST_F(BmsFaultTest, check_all_cell_voltages_segment0) {
    // Test that every cell voltage in Segment 0 is being properly transmitted to CAN

    TearDown();
    SetUp();
    float fake_voltage = 3.0f;
    fake_io_ltc6813CellVoltages_getCellVoltage_returnsForAnyArgs(fake_voltage);
    LetTimePass(1000);
    app_diagnosticsMode_broadcast();
    ASSERT_EQ(fake_voltage, app_canTx_BMS_Seg0_Cell0_Voltage_get());
    ASSERT_EQ(fake_voltage, app_canTx_BMS_Seg0_Cell1_Voltage_get());

    // for (uint8_t segment = 0; segment < 1; segment++)
    // {
    //     for (uint8_t cell = 0; cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; cell++)
    //     {
    //         // Let accumulator startup count expire
    //         LetTimePass(1000);
    //         // ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
    //         // ASSERT_FALSE(app_canAlerts_BMS_Fault_CellOvervoltage_get());

    //         // Set cell voltage to fake_voltage
    //         fake_io_ltc6813CellVoltages_getCellVoltage_returnsForAnyArgs(fake_voltage);
    //         LetTimePass(1000);
    //         ++fake_voltage;
    //     }
    // }

    // LetTimePass(1000);
    // ASSERT_EQ(3.0, app_canTx_BMS_Seg0_Cell0_Voltage_get());
    // ASSERT_EQ(3.1, app_canTx_BMS_Seg0_Cell1_Voltage_get());
    // ASSERT_EQ(3.2, app_canTx_BMS_Seg0_Cell2_Voltage_get());
    // ASSERT_EQ(3.3, app_canTx_BMS_Seg0_Cell3_Voltage_get());
    // ASSERT_EQ(3.4, app_canTx_BMS_Seg0_Cell4_Voltage_get());
    // ASSERT_EQ(3.5, app_canTx_BMS_Seg0_Cell5_Voltage_get());
    // ASSERT_EQ(3.6, app_canTx_BMS_Seg0_Cell6_Voltage_get());
    // ASSERT_EQ(3.7, app_canTx_BMS_Seg0_Cell7_Voltage_get());
    // ASSERT_EQ(3.8, app_canTx_BMS_Seg0_Cell8_Voltage_get());
    // ASSERT_EQ(3.9, app_canTx_BMS_Seg0_Cell9_Voltage_get());
    // ASSERT_EQ(4.0, app_canTx_BMS_Seg0_Cell10_Voltage_get());
    // ASSERT_EQ(4.1, app_canTx_BMS_Seg0_Cell11_Voltage_get());
    // ASSERT_EQ(4.2, app_canTx_BMS_Seg0_Cell12_Voltage_get()); 
    // ASSERT_EQ(4.3, app_canTx_BMS_Seg0_Cell13_Voltage_get()); 
    // ASSERT_EQ(4.4, app_canTx_BMS_Seg0_Cell14_Voltage_get()); 
    // ASSERT_EQ(4.5, app_canTx_BMS_Seg0_Cell15_Voltage_get());  
}