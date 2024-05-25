#include "test_bmsBaseStateMachineTest.h"

class BmsFaultTest : public BmsBaseStateMachineTest
{
};

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_overvoltage)
{
    bool debounce_expires[2] = { true, false };
    for (int i = 0; i < sizeof(debounce_expires); i++)
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
                LetTimePass(1000);
                ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
                ASSERT_FALSE(app_canAlerts_BMS_Fault_CellOvervoltage_get());

                // Set cell voltage critically high.
                fake_io_ltc6813CellVoltages_getCellVoltage_returnsForAnyArgs(MAX_CELL_VOLTAGE + 0.1f);
                LetTimePass(OVER_VOLTAGE_DEBOUNCE_DURATION_MS);

                if (debounce_expires[i])
                {
                    // Let fault debounce expire, fault should be set.
                    LetTimePass(10);
                    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
                    ASSERT_TRUE(app_canAlerts_BMS_Fault_CellOvervoltage_get());

                    LetTimePass(1000);
                    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
                    ASSERT_TRUE(app_canAlerts_BMS_Fault_CellOvervoltage_get());

                    // Clear fault, should transition back to init
                    fake_io_ltc6813CellVoltages_getCellVoltage_returnsForAnyArgs(MAX_CELL_VOLTAGE - 0.1f);
                    LetTimePass(20);
                    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
                    ASSERT_FALSE(app_canAlerts_BMS_Fault_CellOvervoltage_get());
                }
                else
                {
                    // Clear fault before it expires, fault should not set.
                    fake_io_ltc6813CellVoltages_getCellVoltage_returnsForAnyArgs(MAX_CELL_VOLTAGE - 0.1f);
                    LetTimePass(10);
                    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
                    ASSERT_FALSE(app_canAlerts_BMS_Fault_CellOvervoltage_get());

                    LetTimePass(1000);
                    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
                    ASSERT_FALSE(app_canAlerts_BMS_Fault_CellOvervoltage_get());
                }
            }
        }
    }
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_undervoltage)
{
    bool debounce_expires[2] = { true, false };
    for (int i = 0; i < sizeof(debounce_expires); i++)
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
                LetTimePass(1000);
                ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
                ASSERT_FALSE(app_canAlerts_BMS_Fault_CellUndervoltage_get());

                // Set cell voltage critically low.
                fake_io_ltc6813CellVoltages_getCellVoltage_returnsForAnyArgs(MIN_CELL_VOLTAGE - 0.1f);
                LetTimePass(UNDER_VOLTAGE_DEBOUNCE_DURATION_MS);

                if (debounce_expires[i])
                {
                    // Let fault debounce expire, fault should be set.
                    LetTimePass(10);
                    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
                    ASSERT_TRUE(app_canAlerts_BMS_Fault_CellUndervoltage_get());

                    LetTimePass(1000);
                    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
                    ASSERT_TRUE(app_canAlerts_BMS_Fault_CellUndervoltage_get());

                    // Clear fault, should transition back to init
                    fake_io_ltc6813CellVoltages_getCellVoltage_returnsForAnyArgs(MIN_CELL_VOLTAGE + 0.1f);
                    LetTimePass(20);
                    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
                    ASSERT_FALSE(app_canAlerts_BMS_Fault_CellUndervoltage_get());
                }
                else
                {
                    // Clear fault before it expires, fault should not set.
                    fake_io_ltc6813CellVoltages_getCellVoltage_returnsForAnyArgs(MIN_CELL_VOLTAGE + 0.1f);
                    LetTimePass(10);
                    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
                    ASSERT_FALSE(app_canAlerts_BMS_Fault_CellUndervoltage_get());

                    LetTimePass(1000);
                    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
                    ASSERT_FALSE(app_canAlerts_BMS_Fault_CellUndervoltage_get());
                }
            }
        }
    }
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_overtemp_init_state)
{
    bool debounce_expires[2] = { true, false };
    for (int i = 0; i < sizeof(debounce_expires); i++)
    {
        // Reset test
        TearDown();
        SetUp();

        // Set TS current negative to trigger discharging condition in tempertature check
        fake_io_tractiveSystem_getCurrentHighResolution_returns(-10.0f);
        fake_io_tractiveSystem_getCurrentLowResolution_returns(-10.0f);
        SetInitialState(app_initState_get());

        // Let accumulator startup count expire
        LetTimePass(1000);
        ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
        ASSERT_FALSE(app_canAlerts_BMS_Fault_CellOvertemp_get());

        // // In Discharge state, acceptible temp range is (-20, 60), should be unaffected by temp of 46 C
        fake_io_ltc6813CellTemps_getMaxTempDegC_returnsForAnyArgs(MAX_CELL_CHARGE_TEMP_DEGC + 1.0f);
        LetTimePass(1000);
        ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
        ASSERT_FALSE(app_canAlerts_BMS_Fault_CellOvertemp_get());

        // Set cell temperature critically high.
        fake_io_ltc6813CellTemps_getMaxTempDegC_returnsForAnyArgs(MAX_CELL_DISCHARGE_TEMP_DEGC + 1.0f);
        LetTimePass(OVER_TEMP_DEBOUNCE_DURATION_MS);

        if (debounce_expires[i])
        {
            // Let fault debounce expire, fault should be set.
            LetTimePass(10);
            ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
            ASSERT_TRUE(app_canAlerts_BMS_Fault_CellOvertemp_get());

            // Confirm stays in fault indefinitely
            LetTimePass(1000);
            ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
            ASSERT_TRUE(app_canAlerts_BMS_Fault_CellOvertemp_get());

            // Clear fault, should transition back to init
            fake_io_ltc6813CellTemps_getMaxTempDegC_returnsForAnyArgs(MAX_CELL_DISCHARGE_TEMP_DEGC - 1.0f);
            fake_io_airs_isNegativeClosed_returns(false);
            fake_io_charger_hasFaulted_returns(false);

            LetTimePass(10);
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
            ASSERT_FALSE(app_canAlerts_BMS_Fault_CellOvertemp_get());
        }
        else
        {
            // Clear fault before it expires, fault should not set.
            fake_io_ltc6813CellTemps_getMaxTempDegC_returnsForAnyArgs(MAX_CELL_DISCHARGE_TEMP_DEGC - 1.0f);
            LetTimePass(10);
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
            ASSERT_FALSE(app_canAlerts_BMS_Fault_CellOvertemp_get());

            LetTimePass(1000);
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
            ASSERT_FALSE(app_canAlerts_BMS_Fault_CellOvertemp_get());
        }
    }
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_overtemp_charge_state)
{
    bool debounce_expires[2] = { true, false };
    for (int i = 0; i < sizeof(debounce_expires); i++)
    {
        // Reset test
        TearDown();
        SetUp();

        // Set charger conditions such that charger faults do not trigger
        fake_io_charger_isConnected_returns(true);
        fake_io_airs_isNegativeClosed_returns(true);
        fake_io_charger_hasFaulted_returns(false);

        // Set TS current positive to trigger charging condition in temperature check
        fake_io_tractiveSystem_getCurrentHighResolution_returns(10.0f);
        fake_io_tractiveSystem_getCurrentLowResolution_returns(10.0f);

        SetInitialState(app_chargeState_get());
        app_canRx_Debug_StartCharging_update(true);

        // Let accumulator startup count expire
        LetTimePass(1000);
        ASSERT_EQ(app_chargeState_get(), app_stateMachine_getCurrentState());
        ASSERT_FALSE(app_canAlerts_BMS_Fault_CellOvertemp_get());

        // In Charge state acceptible temp range is (0, 45)
        fake_io_ltc6813CellTemps_getMaxTempDegC_returnsForAnyArgs(MAX_CELL_CHARGE_TEMP_DEGC + 1.0f);
        LetTimePass(OVER_TEMP_DEBOUNCE_DURATION_MS);

        if (debounce_expires[i])
        {
            // Let fault debounce expire, fault should be set.
            LetTimePass(10);
            ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
            ASSERT_TRUE(app_canAlerts_BMS_Fault_CellOvertemp_get());

            // Confirm stays in fault indefinitely
            LetTimePass(1000);
            ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
            ASSERT_TRUE(app_canAlerts_BMS_Fault_CellOvertemp_get());

            // Clear fault, should transition back to init
            fake_io_ltc6813CellTemps_getMaxTempDegC_returnsForAnyArgs(MAX_CELL_CHARGE_TEMP_DEGC - 1.0f);
            fake_io_airs_isNegativeClosed_returns(false); // Negative contactor has to open to go back to init
            LetTimePass(10);
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
            ASSERT_FALSE(app_canAlerts_BMS_Fault_CellOvertemp_get());
        }
        else
        {
            // Clear fault before it expires, fault should not set.
            fake_io_ltc6813CellTemps_getMaxTempDegC_returnsForAnyArgs(MAX_CELL_CHARGE_TEMP_DEGC - 1.0f);
            LetTimePass(10);
            ASSERT_EQ(app_chargeState_get(), app_stateMachine_getCurrentState());
            ASSERT_FALSE(app_canAlerts_BMS_Fault_CellOvertemp_get());

            LetTimePass(1000);
            ASSERT_EQ(app_chargeState_get(), app_stateMachine_getCurrentState());
            ASSERT_FALSE(app_canAlerts_BMS_Fault_CellOvertemp_get());
        }
    }
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_undertemp_init_state)
{
    bool debounce_expires[2] = { true, false };
    for (int i = 0; i < sizeof(debounce_expires); i++)
    {
        // Reset test
        TearDown();
        SetUp();

        // Set TS current negative to trigger discharging condition in tempertature check
        fake_io_tractiveSystem_getCurrentHighResolution_returns(-10.0f);
        fake_io_tractiveSystem_getCurrentLowResolution_returns(-10.0f);
        fake_io_airs_isNegativeClosed_returns(false);

        SetInitialState(app_initState_get());

        // Let accumulator startup count expire
        LetTimePass(1000);
        ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
        ASSERT_FALSE(app_canAlerts_BMS_Fault_CellUndertemp_get());

        // In Discharge state, acceptible temp range is (-20, 60), should be unaffected by temp of -1 C
        fake_io_ltc6813CellTemps_getMinTempDegC_returnsForAnyArgs(MIN_CELL_CHARGE_TEMP_DEGC - 1.0f);
        LetTimePass(1000);
        ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
        ASSERT_FALSE(app_canAlerts_BMS_Fault_CellUndertemp_get());

        // Set cell temperature critically low.
        fake_io_ltc6813CellTemps_getMinTempDegC_returnsForAnyArgs(MIN_CELL_DISCHARGE_TEMP_DEGC - 1.0f);
        LetTimePass(UNDER_TEMP_DEBOUNCE_DURATION_MS);

        if (debounce_expires[i])
        {
            // Let fault debounce expire, fault should be set.
            LetTimePass(10);
            ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
            ASSERT_TRUE(app_canAlerts_BMS_Fault_CellUndertemp_get());

            // Confirm stays in fault indefinitely
            LetTimePass(1000);
            ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
            ASSERT_TRUE(app_canAlerts_BMS_Fault_CellUndertemp_get());

            // Clear fault, should transition back to init
            fake_io_ltc6813CellTemps_getMinTempDegC_returnsForAnyArgs(MIN_CELL_DISCHARGE_TEMP_DEGC + 1.0f);
            fake_io_airs_isNegativeClosed_returns(false); // Negative contactor has to open to go back to init
            LetTimePass(10);
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
            ASSERT_FALSE(app_canAlerts_BMS_Fault_CellUndertemp_get());
        }
        else
        {
            // Clear fault before it expires, fault should not set.
            fake_io_ltc6813CellTemps_getMinTempDegC_returnsForAnyArgs(MIN_CELL_DISCHARGE_TEMP_DEGC + 1.0f);
            LetTimePass(10);
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
            ASSERT_FALSE(app_canAlerts_BMS_Fault_CellUndertemp_get());

            LetTimePass(1000);
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
            ASSERT_FALSE(app_canAlerts_BMS_Fault_CellUndertemp_get());
        }
    }
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_undertemp_charge_state)
{
    bool debounce_expires[2] = { true, false };
    for (int i = 0; i < sizeof(debounce_expires); i++)
    {
        // Reset test
        TearDown();
        SetUp();

        // Set charger conditions such that charger faults do not trigger
        fake_io_charger_isConnected_returns(true);
        fake_io_charger_hasFaulted_returns(false);
        fake_io_airs_isNegativeClosed_returns(true);
        app_canRx_Debug_StartCharging_update(true);

        // Set TS current positive to trigger charging condition in tempertature check and above threshold to remain
        // charging
        fake_io_tractiveSystem_getCurrentHighResolution_returns(10.0f);
        fake_io_tractiveSystem_getCurrentLowResolution_returns(10.0f);

        SetInitialState(app_chargeState_get());

        // Let accumulator startup count expire
        LetTimePass(1000);
        ASSERT_EQ(app_chargeState_get(), app_stateMachine_getCurrentState());
        ASSERT_FALSE(app_canAlerts_BMS_Fault_CellUndertemp_get());

        // In Charge state acceptable temp range is (0, 45)
        fake_io_ltc6813CellTemps_getMinTempDegC_returnsForAnyArgs(MIN_CELL_CHARGE_TEMP_DEGC - 1.0f);
        LetTimePass(UNDER_TEMP_DEBOUNCE_DURATION_MS);

        if (debounce_expires[i])
        {
            // Let fault debounce expire, fault should be set.
            LetTimePass(10);
            ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
            ASSERT_TRUE(app_canAlerts_BMS_Fault_CellUndertemp_get());

            // Confirm stays in fault indefinitely
            LetTimePass(1000);
            ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
            ASSERT_TRUE(app_canAlerts_BMS_Fault_CellUndertemp_get());

            // Clear fault, should transition back to init
            fake_io_ltc6813CellTemps_getMinTempDegC_returnsForAnyArgs(MIN_CELL_CHARGE_TEMP_DEGC + 1.0f);
            fake_io_airs_isNegativeClosed_returns(false); // Negative contactor has to open to go back to init
            LetTimePass(10);
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
            ASSERT_FALSE(app_canAlerts_BMS_Fault_CellUndertemp_get());
        }
        else
        {
            // Clear fault before it expires, fault should not set.
            fake_io_ltc6813CellTemps_getMinTempDegC_returnsForAnyArgs(MIN_CELL_CHARGE_TEMP_DEGC + 1.0f);
            LetTimePass(10);
            ASSERT_EQ(app_chargeState_get(), app_stateMachine_getCurrentState());
            ASSERT_FALSE(app_canAlerts_BMS_Fault_CellUndertemp_get());

            LetTimePass(1000);
            ASSERT_EQ(app_chargeState_get(), app_stateMachine_getCurrentState());
            ASSERT_FALSE(app_canAlerts_BMS_Fault_CellUndertemp_get());
        }
    }
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_ts_discharge_overcurrent)
{
    bool debounce_expires[2] = { true, false };
    for (int i = 0; i < sizeof(debounce_expires); i++)
    {
        // Reset test
        TearDown();
        SetUp();

        // Set TS current negative to trigger discharging condition
        fake_io_tractiveSystem_getCurrentHighResolution_returns(-10.0f);
        fake_io_tractiveSystem_getCurrentLowResolution_returns(-10.0f);

        SetInitialState(app_initState_get());

        // Let accumulator startup count expire
        LetTimePass(1000);
        ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
        ASSERT_FALSE(app_canAlerts_BMS_Fault_TractiveSystemOvercurrent_get());

        // Max acceptable discharge current is -88.5A*3 = -265.5A
        fake_io_tractiveSystem_getCurrentHighResolution_returns(MAX_TS_DISCHARGE_CURRENT_AMPS - 1.0f);
        fake_io_tractiveSystem_getCurrentLowResolution_returns(MAX_TS_DISCHARGE_CURRENT_AMPS - 1.0f);
        LetTimePass(TS_OVERCURRENT_DEBOUNCE_DURATION_MS);

        if (debounce_expires[i])
        {
            // Let fault debounce expire, fault should be set.
            LetTimePass(10);
            ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
            ASSERT_TRUE(app_canAlerts_BMS_Fault_TractiveSystemOvercurrent_get());

            // Confirm stays in fault indefinitely
            LetTimePass(1000);
            ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
            ASSERT_TRUE(app_canAlerts_BMS_Fault_TractiveSystemOvercurrent_get());

            // Clear fault, should transition back to init
            fake_io_tractiveSystem_getCurrentHighResolution_returns(MAX_TS_DISCHARGE_CURRENT_AMPS + 1.0f);
            fake_io_tractiveSystem_getCurrentLowResolution_returns(MAX_TS_DISCHARGE_CURRENT_AMPS + 1.0f);
            LetTimePass(10);
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
            ASSERT_FALSE(app_canAlerts_BMS_Fault_TractiveSystemOvercurrent_get());
        }
        else
        {
            // Clear fault before it expires, fault should not set.
            fake_io_tractiveSystem_getCurrentHighResolution_returns(MAX_TS_DISCHARGE_CURRENT_AMPS + 1.0f);
            fake_io_tractiveSystem_getCurrentLowResolution_returns(MAX_TS_DISCHARGE_CURRENT_AMPS + 1.0f);
            LetTimePass(10);
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
            ASSERT_FALSE(app_canAlerts_BMS_Fault_TractiveSystemOvercurrent_get());

            LetTimePass(1000);
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
            ASSERT_FALSE(app_canAlerts_BMS_Fault_TractiveSystemOvercurrent_get());
        }
    }
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_ts_charge_overcurrent)
{
    bool debounce_expires[2] = { true, false };
    for (int i = 0; i < sizeof(debounce_expires); i++)
    {
        // Reset test
        TearDown();
        SetUp();

        // Set charger conditions such that charger faults do not trigger
        fake_io_charger_isConnected_returns(true);
        fake_io_charger_hasFaulted_returns(false);
        fake_io_airs_isNegativeClosed_returns(true);
        app_canRx_Debug_StartCharging_update(true);

        // Set TS current above cutoff threshold to keep state machine in charge state
        fake_io_tractiveSystem_getCurrentHighResolution_returns(10.0f);
        fake_io_tractiveSystem_getCurrentLowResolution_returns(10.0f);

        SetInitialState(app_chargeState_get());

        // Let accumulator startup count expire
        LetTimePass(1000);
        ASSERT_EQ(app_chargeState_get(), app_stateMachine_getCurrentState());
        ASSERT_FALSE(app_canAlerts_BMS_Fault_TractiveSystemOvercurrent_get());

        // Max acceptable charge current is 23.6A * 3 = 70.8A
        fake_io_tractiveSystem_getCurrentHighResolution_returns(MAX_TS_CHARGE_CURRENT_AMPS + 1.0f);
        fake_io_tractiveSystem_getCurrentLowResolution_returns(MAX_TS_CHARGE_CURRENT_AMPS + 1.0f);
        LetTimePass(TS_OVERCURRENT_DEBOUNCE_DURATION_MS);

        if (debounce_expires[i])
        {
            // Let fault debounce expire, fault should be set.
            LetTimePass(10);
            ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
            ASSERT_TRUE(app_canAlerts_BMS_Fault_TractiveSystemOvercurrent_get());

            // Confirm stays in fault indefinitely
            LetTimePass(1000);
            ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
            ASSERT_TRUE(app_canAlerts_BMS_Fault_TractiveSystemOvercurrent_get());

            // Clear fault, should transition back to init
            fake_io_tractiveSystem_getCurrentHighResolution_returns(MAX_TS_CHARGE_CURRENT_AMPS - 1.0f);
            fake_io_tractiveSystem_getCurrentLowResolution_returns(MAX_TS_CHARGE_CURRENT_AMPS - 1.0f);
            fake_io_airs_isNegativeClosed_returns(false); // Negative contactor has to open to go back to init
            LetTimePass(10);
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
            ASSERT_FALSE(app_canAlerts_BMS_Fault_TractiveSystemOvercurrent_get());
        }
        else
        {
            // Clear fault before it expires, fault should not set.
            fake_io_tractiveSystem_getCurrentHighResolution_returns(MAX_TS_CHARGE_CURRENT_AMPS - 1.0f);
            fake_io_tractiveSystem_getCurrentLowResolution_returns(MAX_TS_CHARGE_CURRENT_AMPS - 1.0f);
            LetTimePass(10);
            ASSERT_EQ(app_chargeState_get(), app_stateMachine_getCurrentState());
            ASSERT_FALSE(app_canAlerts_BMS_Fault_TractiveSystemOvercurrent_get());

            LetTimePass(1000);
            ASSERT_EQ(app_chargeState_get(), app_stateMachine_getCurrentState());
            ASSERT_FALSE(app_canAlerts_BMS_Fault_TractiveSystemOvercurrent_get());
        }
    }
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
        app_canRx_Debug_StartCharging_update(false);
        LetTimePass(210U);
        ASSERT_EQ(app_prechargeState_get(), app_stateMachine_getCurrentState());
        ASSERT_FALSE(app_canAlerts_BMS_Fault_PrechargeFailure_get());

        // 3.8V nominal cell voltage * total # of cells to give estimate of nominal pack voltage
        // trying to fool precahrge into thinking that ts_voltage is rising too quickly
        fake_io_tractiveSystem_getVoltage_returns(3.8f * ACCUMULATOR_NUM_SERIES_CELLS_TOTAL);

        LetTimePass(210U);

        if (i < 3)
        {
            // 3x precharge attempts haven't been exceeded, so back to init
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
            ASSERT_FALSE(app_canAlerts_BMS_Fault_PrechargeFailure_get());

            // reset ts_voltage to 0 so state will transition from init to pre-charge
            fake_io_tractiveSystem_getVoltage_returns(0);
        }
        else
        {
            // 3x precharge attempts have failed, so back transition to fault state indefinitely
            ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
            ASSERT_TRUE(app_canAlerts_BMS_Fault_PrechargeFailure_get());
            LetTimePass(1000U);
            ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
            ASSERT_TRUE(app_canAlerts_BMS_Fault_PrechargeFailure_get());

            // Can't transition out of fault state due to precharge failure!
        }
    }
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_disables_bms_ok)
{
    fake_io_faultLatch_setCurrentStatus_reset();

    // Let accumulator startup count expire
    LetTimePass(1000);
    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
    ASSERT_EQ(fake_io_faultLatch_setCurrentStatus_callCountForArgs(&bms_ok_latch, true), 0);
    ASSERT_EQ(fake_io_faultLatch_setCurrentStatus_callCountForArgs(&bms_ok_latch, false), 0);

    // Set cell voltage critically high and confirm fault is set
    fake_io_ltc6813CellVoltages_getCellVoltage_returnsForAnyArgs(MAX_CELL_VOLTAGE + 0.1f);
    LetTimePass(OVER_VOLTAGE_DEBOUNCE_DURATION_MS + 10);
    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());

    // BMS OK status should be disabled upon entering fault state, which is latched in hardware
    ASSERT_EQ(fake_io_faultLatch_setCurrentStatus_callCountForArgs(&bms_ok_latch, true), 0);
    ASSERT_EQ(fake_io_faultLatch_setCurrentStatus_callCountForArgs(&bms_ok_latch, false), 1);
}
