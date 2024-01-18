#include "test_bmsBaseStateMachineTest.h"

class BmsFaultTest : public BmsBaseStateMachineTest
{
};

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_overvoltage)
{
    // Test that any cell can cause an overvoltage fault
    for (uint8_t segment = 0; segment < ACCUMULATOR_NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0; cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; cell++)
        {
            // Reset test
            TearDown();
            SetUp();

            // Let accumulator startup count expire
            LetTimePass(state_machine, 1000);
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState(state_machine));
            ASSERT_FALSE(App_CanAlerts_BMS_Fault_CellOvervoltage_Get());

            // Set cell voltage critically high and confirm fault is set
            fake_io_ltc6813CellVoltages_getCellVoltage_returnsForAnyArgs(MAX_CELL_VOLTAGE + 0.1f);
            LetTimePass(state_machine, 20);
            ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState(state_machine));
            ASSERT_TRUE(App_CanAlerts_BMS_Fault_CellOvervoltage_Get());

            LetTimePass(state_machine, 1000);
            ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState(state_machine));
            ASSERT_TRUE(App_CanAlerts_BMS_Fault_CellOvervoltage_Get());

            // Clear fault, should transition back to init
            fake_io_ltc6813CellVoltages_getCellVoltage_returnsForAnyArgs(MAX_CELL_VOLTAGE - 0.1f);
            LetTimePass(state_machine, 20);
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState(state_machine));
            ASSERT_FALSE(App_CanAlerts_BMS_Fault_CellOvervoltage_Get());
        }
    }
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_undervoltage)
{
    // Test that any cell can cause an undervoltage fault
    for (uint8_t segment = 0; segment < ACCUMULATOR_NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0; cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; cell++)
        {
            // Reset test
            TearDown();
            SetUp();

            // Let accumulator startup count expire
            LetTimePass(state_machine, 1000);
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState(state_machine));
            ASSERT_FALSE(App_CanAlerts_BMS_Fault_CellUndervoltage_Get());

            // Set cell voltage critically low and confirm fault is set
            fake_io_ltc6813CellVoltages_getCellVoltage_returnsForAnyArgs(MIN_CELL_VOLTAGE - 0.1f);
            LetTimePass(state_machine, 20);
            ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState(state_machine));
            ASSERT_TRUE(App_CanAlerts_BMS_Fault_CellUndervoltage_Get());

            LetTimePass(state_machine, 1000);
            ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState(state_machine));
            ASSERT_TRUE(App_CanAlerts_BMS_Fault_CellUndervoltage_Get());

            // Clear fault, should transition back to init
            fake_io_ltc6813CellVoltages_getCellVoltage_returnsForAnyArgs(MIN_CELL_VOLTAGE + 0.1f);
            LetTimePass(state_machine, 20);
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState(state_machine));
            ASSERT_FALSE(App_CanAlerts_BMS_Fault_CellUndervoltage_Get());
        }
    }
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_overtemp_init_state)
{
    // Set TS current negative to trigger discharging condition in tempertature check
    fake_io_tractiveSystem_getCurrentHighResolution_returns(-10.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(-10.0f);
    fake_io_airs_isNegativeClosed_returns(true);

    SetInitialState(app_initState_get());

    fake_io_charger_isConnected_returns(true);
    App_CanRx_Debug_StartCharging_Update(false);

    // Let accumulator startup count expire
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_BMS_Fault_CellOvertemp_Get());

    // In Discharge state, acceptible temp range is (-20, 60), should be unaffected by temp of 46 C
    fake_io_ltc6813CellTemps_getMaxTempDegC_returnsForAnyArgs(MAX_CELL_CHARGE_TEMP_DEGC + 1.0f);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_BMS_Fault_CellOvertemp_Get());

    LetTimePass(state_machine, 1000);

    fake_io_ltc6813CellTemps_getMaxTempDegC_returnsForAnyArgs(MAX_CELL_DISCHARGE_TEMP_DEGC + 1.0f);
    LetTimePass(state_machine, 10);

    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_BMS_Fault_CellOvertemp_Get());

    // Confirm stays in fault indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_BMS_Fault_CellOvertemp_Get());

    // Clear fault, should transition back to init
    fake_io_ltc6813CellTemps_getMaxTempDegC_returnsForAnyArgs(MAX_CELL_DISCHARGE_TEMP_DEGC - 1.0f);
    fake_io_airs_isNegativeClosed_returns(false);
    fake_io_charger_hasFaulted_returns(false);

    LetTimePass(state_machine, 10);
    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_BMS_Fault_CellOvertemp_Get());
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_overtemp_charge_state)
{
    // Set charger conditions such that charger faults do not trigger
    fake_io_charger_isConnected_returns(true);
    fake_io_airs_isNegativeClosed_returns(true);
    fake_io_charger_hasFaulted_returns(false);

    // Set TS current positive to trigger charging condition in temperature check
    fake_io_tractiveSystem_getCurrentHighResolution_returns(10.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(10.0f);

    SetInitialState(app_chargeState_get());
    App_CanRx_Debug_StartCharging_Update(true);

    // Let accumulator startup count expire
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(app_chargeState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_BMS_Fault_CellOvertemp_Get());

    // In Charge state acceptible temp range is (0, 45)
    fake_io_ltc6813CellTemps_getMaxTempDegC_returnsForAnyArgs(MAX_CELL_CHARGE_TEMP_DEGC + 1.0f);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_BMS_Fault_CellOvertemp_Get());

    // Confirm stays in fault indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_BMS_Fault_CellOvertemp_Get());

    // Clear fault, should transition back to init
    fake_io_ltc6813CellTemps_getMaxTempDegC_returnsForAnyArgs(MAX_CELL_CHARGE_TEMP_DEGC - 1.0f);
    fake_io_airs_isNegativeClosed_returns(false); // Negative contactor has to open to go back to init
    LetTimePass(state_machine, 10);
    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_BMS_Fault_CellOvertemp_Get());
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_undertemp_init_state)
{
    // Set TS current negative to trigger discharging condition in tempertature check
    fake_io_tractiveSystem_getCurrentHighResolution_returns(-10.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(-10.0f);
    fake_io_airs_isNegativeClosed_returns(false);

    SetInitialState(app_initState_get());

    // Let accumulator startup count expire
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_BMS_Fault_CellUndertemp_Get());

    // In Discharge state, acceptible temp range is (-20, 60), should be unaffected by temp of -1 C
    fake_io_ltc6813CellTemps_getMinTempDegC_returnsForAnyArgs(MIN_CELL_CHARGE_TEMP_DEGC - 1.0f);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_BMS_Fault_CellUndertemp_Get());

    LetTimePass(state_machine, 1000);

    fake_io_ltc6813CellTemps_getMinTempDegC_returnsForAnyArgs(MIN_CELL_DISCHARGE_TEMP_DEGC - 1.0f);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_BMS_Fault_CellUndertemp_Get());

    // Confirm stays in fault indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_BMS_Fault_CellUndertemp_Get());

    // Clear fault, should transition back to init
    fake_io_ltc6813CellTemps_getMinTempDegC_returnsForAnyArgs(MIN_CELL_DISCHARGE_TEMP_DEGC + 1.0f);
    fake_io_airs_isNegativeClosed_returns(false); // Negative contactor has to open to go back to init
    LetTimePass(state_machine, 10);
    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_BMS_Fault_CellUndertemp_Get());
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_undertemp_charge_state)
{
    // Set charger conditions such that charger faults do not trigger
    fake_io_charger_isConnected_returns(true);
    fake_io_charger_hasFaulted_returns(false);
    fake_io_airs_isNegativeClosed_returns(true);
    App_CanRx_Debug_StartCharging_Update(true);

    // Set TS current positive to trigger charging condition in tempertature check and above threshold to remain
    // charging
    fake_io_tractiveSystem_getCurrentHighResolution_returns(10.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(10.0f);

    SetInitialState(app_chargeState_get());

    // Let accumulator startup count expire
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(app_chargeState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_BMS_Fault_CellUndertemp_Get());

    // In Charge state acceptable temp range is (0, 45)
    fake_io_ltc6813CellTemps_getMinTempDegC_returnsForAnyArgs(MIN_CELL_CHARGE_TEMP_DEGC - 1.0f);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_BMS_Fault_CellUndertemp_Get());

    // Confirm stays in fault indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_BMS_Fault_CellUndertemp_Get());

    // Clear fault, should transition back to init
    fake_io_ltc6813CellTemps_getMinTempDegC_returnsForAnyArgs(MIN_CELL_CHARGE_TEMP_DEGC + 1.0f);
    fake_io_airs_isNegativeClosed_returns(false); // Negative contactor has to open to go back to init
    LetTimePass(state_machine, 10);
    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_BMS_Fault_CellUndertemp_Get());
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_ts_discharge_overcurrent)
{
    // Set TS current negative to trigger discharging condition
    fake_io_tractiveSystem_getCurrentHighResolution_returns(-10.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(-10.0f);

    SetInitialState(app_initState_get());

    // Let accumulator startup count expire
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_BMS_Fault_TractiveSystemOvercurrent_Get());

    // Max acceptable discharge current is -88.5A*3 = -265.5A
    fake_io_tractiveSystem_getCurrentHighResolution_returns(MAX_TS_DISCHARGE_CURRENT_AMPS - 1.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(MAX_TS_DISCHARGE_CURRENT_AMPS - 1.0f);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_BMS_Fault_TractiveSystemOvercurrent_Get());

    // Confirm stays in fault indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_BMS_Fault_TractiveSystemOvercurrent_Get());

    // Clear fault, should transition back to init
    fake_io_tractiveSystem_getCurrentHighResolution_returns(MAX_TS_DISCHARGE_CURRENT_AMPS + 1.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(MAX_TS_DISCHARGE_CURRENT_AMPS + 1.0f);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_BMS_Fault_TractiveSystemOvercurrent_Get());
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_ts_charge_overcurrent)
{
    // Set charger conditions such that charger faults do not trigger
    fake_io_charger_isConnected_returns(true);
    fake_io_charger_hasFaulted_returns(false);
    fake_io_airs_isNegativeClosed_returns(true);
    App_CanRx_Debug_StartCharging_Update(true);

    // Set TS current above cutoff threshold to keep state machine in charge state
    fake_io_tractiveSystem_getCurrentHighResolution_returns(10.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(10.0f);

    SetInitialState(app_chargeState_get());

    // Let accumulator startup count expire
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(app_chargeState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_BMS_Fault_TractiveSystemOvercurrent_Get());

    // Max acceptable charge current is 23.6A * 3 = 70.8A
    fake_io_tractiveSystem_getCurrentHighResolution_returns(MAX_TS_CHARGE_CURRENT_AMPS + 1.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(MAX_TS_CHARGE_CURRENT_AMPS + 1.0f);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_BMS_Fault_TractiveSystemOvercurrent_Get());

    // Confirm stays in fault indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_BMS_Fault_TractiveSystemOvercurrent_Get());

    // Clear fault, should transition back to init
    fake_io_tractiveSystem_getCurrentHighResolution_returns(MAX_TS_CHARGE_CURRENT_AMPS - 1.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(MAX_TS_CHARGE_CURRENT_AMPS - 1.0f);
    fake_io_airs_isNegativeClosed_returns(false); // Negative contactor has to open to go back to init
    LetTimePass(state_machine, 10);
    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_BMS_Fault_TractiveSystemOvercurrent_Get());
}

TEST_F(BmsFaultTest, check_state_transition_fault_state_precharge_fault)
{
    SetInitialState(app_initState_get());

    // reset ts_voltage to 0 so state will transition from init to pre-charge
    fake_io_tractiveSystem_getVoltage_returns(0);

    for (int i = 1; i <= 3; i++)
    {
        // Close negative contactor with charger disconnected, precharge should start
        fake_io_charger_isConnected_returns(false);
        fake_io_airs_isNegativeClosed_returns(true);
        App_CanRx_Debug_StartCharging_Update(false);
        LetTimePass(state_machine, 210U);
        ASSERT_EQ(app_prechargeState_get(), app_stateMachine_getCurrentState(state_machine));
        ASSERT_FALSE(App_CanAlerts_BMS_Fault_PrechargeFailure_Get());

        // 3.8V nominal cell voltage * total # of cells to give estimate of nominal pack voltage
        // trying to fool precahrge into thinking that ts_voltage is rising too quickly
        fake_io_tractiveSystem_getVoltage_returns(3.8f * ACCUMULATOR_NUM_SERIES_CELLS_TOTAL);

        LetTimePass(state_machine, 210U);

        if (i < 3)
        {
            // 3x precharge attempts haven't been exceeded, so back to init
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState(state_machine));
            ASSERT_FALSE(App_CanAlerts_BMS_Fault_PrechargeFailure_Get());

            // reset ts_voltage to 0 so state will transition from init to pre-charge
            fake_io_tractiveSystem_getVoltage_returns(0);
        }
        else
        {
            // 3x precharge attempts have failed, so back transition to fault state indefinitely
            ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState(state_machine));
            ASSERT_TRUE(App_CanAlerts_BMS_Fault_PrechargeFailure_Get());
            LetTimePass(state_machine, 1000U);
            ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState(state_machine));
            ASSERT_TRUE(App_CanAlerts_BMS_Fault_PrechargeFailure_Get());

            // Can't transition out of fault state due to precharge failure!
        }
    }
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_disables_bms_ok)
{
    fake_io_faultLatch_setCurrentStatus_reset();

    // Let accumulator startup count expire
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_EQ(fake_io_faultLatch_setCurrentStatus_callCountForArgs(&bms_ok_latch, true), 0);
    ASSERT_EQ(fake_io_faultLatch_setCurrentStatus_callCountForArgs(&bms_ok_latch, false), 0);

    // Set cell voltage critically high and confirm fault is set
    fake_io_ltc6813CellVoltages_getCellVoltage_returnsForAnyArgs(MAX_CELL_VOLTAGE + 0.1f);
    LetTimePass(state_machine, 20);
    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState(state_machine));

    // BMS OK status should be disabled upon entering fault state, which is latched in hardware
    ASSERT_EQ(fake_io_faultLatch_setCurrentStatus_callCountForArgs(&bms_ok_latch, true), 0);
    ASSERT_EQ(fake_io_faultLatch_setCurrentStatus_callCountForArgs(&bms_ok_latch, false), 1);
}
