#include "test_bmsBaseStateMachineTest.h"
#include "test_imd.h"

class BmsStateMachineTest : public BmsBaseStateMachineTest
{
};

TEST_F(BmsStateMachineTest, check_init_state_is_broadcasted_over_can)
{
    SetInitialState(app_initState_get());
    EXPECT_EQ(BMS_INIT_STATE, App_CanTx_BMS_State_Get());
}

TEST_F(BmsStateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(app_driveState_get());
    EXPECT_EQ(BMS_DRIVE_STATE, App_CanTx_BMS_State_Get());
}

TEST_F(BmsStateMachineTest, check_fault_state_is_broadcasted_over_can)
{
    SetInitialState(app_faultState_get());
    EXPECT_EQ(BMS_FAULT_STATE, App_CanTx_BMS_State_Get());
}

TEST_F(BmsStateMachineTest, check_charge_state_is_broadcasted_over_can)
{
    SetInitialState(app_chargeState_get());
    EXPECT_EQ(BMS_CHARGE_STATE, App_CanTx_BMS_State_Get());
}

TEST_F(BmsStateMachineTest, check_inverter_on_state_is_broadcasted_over_can)
{
    SetInitialState(app_inverterOnState_get());
    EXPECT_EQ(BMS_INVERTER_ON_STATE, App_CanTx_BMS_State_Get());
}

TEST_F(BmsStateMachineTest, check_state_transition_from_init_to_inverter_to_precharge)
{
    fake_io_charger_isConnected_returns(false);
    fake_io_tractiveSystem_getVoltage_returns(2.0f);
    fake_io_airs_isNegativeClosed_returns(true);
    SetInitialState(app_initState_get());

    LetTimePass(, 200);
    EXPECT_EQ(BMS_INVERTER_ON_STATE, App_CanTx_BMS_State_Get()) << "Expected state: BMS_INVERTER_STATE";

    LetTimePass(, 10);
    EXPECT_EQ(BMS_PRECHARGE_STATE, App_CanTx_BMS_State_Get()) << "Expected state: BMS_PRECHARGE_STATE";
}

TEST_F(BmsStateMachineTest, check_imd_frequency_is_broadcasted_over_can_in_all_states)
{
    float fake_frequency = 0.0f;

    for (const auto &state : GetAllStates())
    {
        SetInitialState(state);
        fake_io_imd_getFrequency_returns(fake_frequency);
        LetTimePass(, 10);

        EXPECT_EQ(fake_frequency, App_CanTx_BMS_ImdFrequency_Get());

        // To avoid false positives, we use a different duty cycle each time
        fake_frequency++;
    }
}

TEST_F(BmsStateMachineTest, check_imd_duty_cycle_is_broadcasted_over_can_in_all_states)
{
    float fake_duty_cycle = 0.0f;

    for (const auto &state : GetAllStates())
    {
        SetInitialState(state);
        fake_io_imd_getDutyCycle_returns(fake_duty_cycle);

        LetTimePass(, 10);

        EXPECT_EQ(fake_duty_cycle, App_CanTx_BMS_ImdDutyCycle_Get());

        // To avoid false positives, we use a different frequency each time
        fake_duty_cycle++;
    }
}

TEST_F(BmsStateMachineTest, check_imd_insulation_resistance_10hz_is_broadcasted_over_can_in_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);
        test_imd_setImdCondition(IMD_NORMAL);

        // Test an arbitrarily chosen valid resistance
        fake_io_imd_getDutyCycle_returns(50.0f);
        LetTimePass(, 10);

        EXPECT_EQ(IMD_NORMAL, App_CanTx_BMS_ImdCondition_Get());
        EXPECT_EQ(true, App_CanTx_BMS_ImdValidDutyCycle_Get());
        EXPECT_EQ(1200, App_CanTx_BMS_ImdInsulationMeasurementDcp10Hz_Get());

        // Test an arbitrarily chosen invalid resistance
        fake_io_imd_getDutyCycle_returns(0.0f);
        LetTimePass(, 10);
        EXPECT_EQ(IMD_NORMAL, App_CanTx_BMS_ImdCondition_Get());
        EXPECT_EQ(false, App_CanTx_BMS_ImdValidDutyCycle_Get());
    }
}

TEST_F(BmsStateMachineTest, check_imd_insulation_resistance_20hz_is_broadcasted_over_can_in_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);
        test_imd_setImdCondition(IMD_UNDERVOLTAGE_DETECTED);

        // Test an arbitrarily chosen valid resistance
        fake_io_imd_getDutyCycle_returns(50.0f);
        LetTimePass(, 10);

        EXPECT_EQ(IMD_UNDERVOLTAGE_DETECTED, App_CanTx_BMS_ImdCondition_Get());
        EXPECT_EQ(true, App_CanTx_BMS_ImdValidDutyCycle_Get());
        EXPECT_EQ(1200, App_CanTx_BMS_ImdInsulationMeasurementDcp20Hz_Get());

        // Test an arbitrarily chosen invalid resistance
        fake_io_imd_getDutyCycle_returns(0.0f);
        LetTimePass(, 10);
        EXPECT_EQ(IMD_UNDERVOLTAGE_DETECTED, App_CanTx_BMS_ImdCondition_Get());
        EXPECT_EQ(false, App_CanTx_BMS_ImdValidDutyCycle_Get());
    }
}

TEST_F(BmsStateMachineTest, check_imd_speed_start_status_30hz_is_broadcasted_over_can_in_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);
        test_imd_setImdCondition(IMD_SST);

        // Test an arbitrarily chosen SST_GOOD
        fake_io_imd_getDutyCycle_returns(7.5f);

        LetTimePass(, 10);
        EXPECT_EQ(IMD_SST, App_CanTx_BMS_ImdCondition_Get());
        EXPECT_EQ(true, App_CanTx_BMS_ImdValidDutyCycle_Get());
        EXPECT_EQ(SST_GOOD, App_CanTx_BMS_ImdSpeedStartStatus30Hz_Get());

        // Test an arbitrarily chosen SST_BAD
        fake_io_imd_getDutyCycle_returns(92.5f);

        LetTimePass(, 10);
        EXPECT_EQ(IMD_SST, App_CanTx_BMS_ImdCondition_Get());
        EXPECT_EQ(true, App_CanTx_BMS_ImdValidDutyCycle_Get());
        EXPECT_EQ(SST_BAD, App_CanTx_BMS_ImdSpeedStartStatus30Hz_Get());

        // Test an arbitrarily chosen invalid SST status
        fake_io_imd_getDutyCycle_returns(0.0f);

        LetTimePass(, 10);
        EXPECT_EQ(IMD_SST, App_CanTx_BMS_ImdCondition_Get());
        EXPECT_EQ(false, App_CanTx_BMS_ImdValidDutyCycle_Get());
    }
}

TEST_F(BmsStateMachineTest, check_imd_seconds_since_power_on_is_broadcasted_over_can_in_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);
        fake_io_imd_getTimeSincePowerOn_returns(123);
        LetTimePass(, 10);
        EXPECT_EQ(123, App_CanTx_BMS_ImdSecondsSincePowerOn_Get());
    }
}

TEST_F(BmsStateMachineTest, charger_connection_status_in_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        fake_io_charger_isConnected_returns(true);
        LetTimePass(, 1000);
        EXPECT_EQ(true, App_CanTx_BMS_ChargerConnected_Get());

        fake_io_charger_isConnected_returns(false);
        LetTimePass(, 1000);
        EXPECT_EQ(false, App_CanTx_BMS_ChargerConnected_Get());
    }
}

TEST_F(BmsStateMachineTest, check_bms_ok_is_broadcasted_over_can_in_all_states)
{
    // Enable BMS_OK
    fake_io_faultLatch_getCurrentStatus_returnsForArgs(&bms_ok_latch, true);

    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        // Make sure the state machine sets the CAN signal for BMS_OK
        LetTimePass(, 10);
        ASSERT_EQ(true, App_CanTx_BMS_BmsOk_Get());

        // Reset the CAN signal for BMS_OK
        App_CanTx_BMS_BmsOk_Set(false);
    }
}

TEST_F(BmsStateMachineTest, check_imd_ok_is_broadcasted_over_can_in_all_states)
{
    // Enable IMD_OK
    fake_io_faultLatch_getCurrentStatus_returnsForArgs(&imd_ok_latch, true);

    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        // Make sure the state machine sets the CAN signal for IMD_OK
        LetTimePass(, 10);
        ASSERT_EQ(true, App_CanTx_BMS_ImdOk_Get());

        // Reset the CAN signal for IMD_OK
        App_CanTx_BMS_ImdOk_Set(false);
    }
}

TEST_F(BmsStateMachineTest, check_bspd_ok_is_broadcasted_over_can_in_all_states)
{
    // Enable BSPD_OK
    fake_io_faultLatch_getCurrentStatus_returnsForArgs(&bspd_ok_latch, true);

    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        // Make sure the state machine sets the CAN signal for BSPD_OK
        LetTimePass(, 10);
        ASSERT_EQ(true, App_CanTx_BMS_BspdOk_Get());

        // Reset the CAN signal for BSPD_OK
        App_CanTx_BMS_BspdOk_Set(false);
    }
}

TEST_F(BmsStateMachineTest, stops_charging_and_faults_if_charger_disconnects_in_charge_state)
{
    SetInitialState(app_chargeState_get());
    fake_io_airs_isNegativeClosed_returns(true);

    // Set the current values to above the threshold for charging to stop (charging should continue)
    fake_io_tractiveSystem_getCurrentHighResolution_returns(1.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(1.0f);

    // Simulate situation with charger present and user indicate to start charging
    fake_io_charger_isConnected_returns(false);
    App_CanRx_Debug_StartCharging_Update(true);

    LetTimePass(, 10);

    // Checks if a CAN message was sent to indicate charger was disconnected unexpectedly
    ASSERT_EQ(true, App_CanAlerts_BMS_Fault_ChargerDisconnectedDuringCharge_Get());
    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
}

TEST_F(BmsStateMachineTest, check_airs_can_signals_for_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        fake_io_airs_isNegativeClosed_returns(false);
        fake_io_airs_isPositiveClosed_returns(false);

        LetTimePass(, 10);
        ASSERT_EQ(false, App_CanTx_BMS_AirNegative_Get());
        ASSERT_EQ(false, App_CanTx_BMS_AirPositive_Get());

        fake_io_airs_isNegativeClosed_returns(false);
        fake_io_airs_isPositiveClosed_returns(true);

        LetTimePass(, 10);
        ASSERT_EQ(false, App_CanTx_BMS_AirNegative_Get());
        ASSERT_EQ(true, App_CanTx_BMS_AirPositive_Get());

        fake_io_airs_isNegativeClosed_returns(true);
        fake_io_airs_isPositiveClosed_returns(false);

        LetTimePass(, 10);
        ASSERT_EQ(true, App_CanTx_BMS_AirNegative_Get());
        ASSERT_EQ(false, App_CanTx_BMS_AirPositive_Get());

        fake_io_airs_isNegativeClosed_returns(true);
        fake_io_airs_isPositiveClosed_returns(true);

        LetTimePass(, 10);
        ASSERT_EQ(true, App_CanTx_BMS_AirNegative_Get());
        ASSERT_EQ(true, App_CanTx_BMS_AirPositive_Get());
    }
}

TEST_F(BmsStateMachineTest, check_contactors_open_in_fault_state)
{
    fake_io_airs_openPositive_reset();

    SetInitialState(app_faultState_get());
    ASSERT_EQ(fake_io_airs_openPositive_callCount(), 1);
}

TEST_F(BmsStateMachineTest, check_state_transition_from_fault_to_init_with_no_faults_set)
{
    // Assume no AIR shutdown faults have been set
    SetInitialState(app_faultState_get());

    // If charger is connected, having air_negative open will lead to fault state, so ensure it is not
    fake_io_charger_isConnected_returns(false);
    fake_io_airs_isNegativeClosed_returns(true);
    LetTimePass(, 1000);
    ASSERT_EQ(BMS_FAULT_STATE, App_CanTx_BMS_State_Get());

    fake_io_airs_isNegativeClosed_returns(false);
    LetTimePass(, 1000);
    ASSERT_EQ(BMS_INIT_STATE, App_CanTx_BMS_State_Get());
}

TEST_F(BmsStateMachineTest, check_state_transition_from_fault_to_init_with_air_negative_open)
{
    SetInitialState(app_faultState_get());

    // Check that state machine remains in FaultState with AIR- closed
    // If charger is connected, having air_negative open will lead to fault state, so ensure it is not
    fake_io_airs_isNegativeClosed_returns(true);
    fake_io_charger_isConnected_returns(false);
    LetTimePass(, 1000);
    ASSERT_EQ(BMS_FAULT_STATE, App_CanTx_BMS_State_Get());

    // Check that state mcachine transitions to InitState with AIR- open
    fake_io_airs_isNegativeClosed_returns(false);
    LetTimePass(, 1000);
    ASSERT_EQ(BMS_INIT_STATE, App_CanTx_BMS_State_Get());
}

TEST_F(BmsStateMachineTest, charger_connected_no_can_msg_init_state)
{
    SetInitialState(app_initState_get());
    fake_io_airs_isNegativeClosed_returns(true);

    // Without the CAN message to start charging, will remain in init state when charger is connected
    fake_io_charger_isConnected_returns(true);

    LetTimePass(, 20);

    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
}

TEST_F(BmsStateMachineTest, charger_connected_can_msg_init_state)
{
    SetInitialState(app_initState_get());
    fake_io_airs_isNegativeClosed_returns(true);

    // Simulate situation with charger present and user indicate to start charging
    fake_io_charger_isConnected_returns(true);
    App_CanRx_Debug_StartCharging_Update(true);

    LetTimePass(, 210U);

    ASSERT_EQ(app_prechargeState_get(), app_stateMachine_getCurrentState());
}

TEST_F(BmsStateMachineTest, charger_connected_successful_precharge_stays)
{
    SetInitialState(app_initState_get());
    fake_io_airs_isNegativeClosed_returns(true);

    // Set the current values to above the threshold for charging to stop (charging should continue)
    fake_io_tractiveSystem_getCurrentHighResolution_returns(1.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(1.0f);

    // Simulate situation with charger present and user indicate to start charging
    fake_io_charger_isConnected_returns(true);
    App_CanRx_Debug_StartCharging_Update(true);

    // Allow BMS time to go through Init state
    LetTimePass(, 210);
    fake_io_tractiveSystem_getVoltage_returns(400.0f);

    // Pause for slightly longer to allow pre-charge
    LetTimePass(, 210);

    printf("%s", app_stateMachine_getCurrentState()->name);
    ASSERT_EQ(app_chargeState_get(), app_stateMachine_getCurrentState());
}

TEST_F(BmsStateMachineTest, keeps_charging_with_no_interrupts)
{
    SetInitialState(app_chargeState_get());
    fake_io_airs_isNegativeClosed_returns(true);

    // Set the current values to above the threshold for charging to stop (charging should continue)
    fake_io_tractiveSystem_getCurrentHighResolution_returns(1.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(1.0f);

    // Simulate situation with charger present and user indicate to start charging
    fake_io_charger_isConnected_returns(true);
    App_CanRx_Debug_StartCharging_Update(true);

    LetTimePass(, 100);

    ASSERT_EQ(fake_io_charger_enable_callCountForArgs(true), 1);
    ASSERT_EQ(app_chargeState_get(), app_stateMachine_getCurrentState());
}

TEST_F(BmsStateMachineTest, stops_charging_after_false_charging_msg)
{
    SetInitialState(app_chargeState_get());
    fake_io_airs_isNegativeClosed_returns(true);

    // Set the current values to above the threshold for charging to stop (charging should continue)
    fake_io_tractiveSystem_getCurrentHighResolution_returns(1.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(1.0f);

    // Simulate situation with charger present and user indicate to start charging
    fake_io_charger_isConnected_returns(true);
    App_CanRx_Debug_StartCharging_Update(false);

    LetTimePass(, 1000);
    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
}

TEST_F(BmsStateMachineTest, fault_from_charger_fault)
{
    SetInitialState(app_chargeState_get());
    fake_io_airs_isNegativeClosed_returns(true);

    // Set the current values to above the threshold for charging to stop (charging should continue)
    fake_io_tractiveSystem_getCurrentHighResolution_returns(1.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(1.0f);

    // Simulate situation with charger present and user indicate to start charging
    fake_io_charger_isConnected_returns(true);
    fake_io_charger_hasFaulted_returns(true);
    App_CanRx_Debug_StartCharging_Update(true);

    // Charger faults are ignored for 5s upon charge state entry
    LetTimePass(, 5010);
    const State *currentState = app_stateMachine_getCurrentState();

    ASSERT_EQ(app_faultState_get(), currentState);
}

TEST_F(BmsStateMachineTest, faults_after_shutdown_loop_activates_while_charging)
{
    SetInitialState(app_chargeState_get());
    fake_io_airs_isNegativeClosed_returns(true);

    // Set the current values to above the threshold for charging to stop (charging should continue)
    fake_io_tractiveSystem_getCurrentHighResolution_returns(1.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(1.0f);

    // Simulate situation with charger present and user indicate to start charging
    fake_io_charger_isConnected_returns(true);
    App_CanRx_Debug_StartCharging_Update(true);

    LetTimePass(, 10);

    // Set current to high value for a hard-coded fault that remains uncleared
    fake_io_tractiveSystem_getCurrentHighResolution_returns(1000.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(1000.0f);
    fake_io_airs_isNegativeClosed_returns(false);

    LetTimePass(, 20);

    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
}

TEST_F(BmsStateMachineTest, check_remains_in_fault_state_until_fault_cleared_then_transitions_to_init)
{
    SetInitialState(app_initState_get());

    // Let accumulator startup count expire
    LetTimePass(, 1000);

    // Set TS current negative to trigger discharging condition in tempertature check
    fake_io_tractiveSystem_getCurrentHighResolution_returns(-10.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(-10.0f);

    // Simulate over-temp fault in drive state
    fake_io_ltc6813CellTemps_getMaxTempDegC_returnsForAnyArgs(MAX_CELL_DISCHARGE_TEMP_DEGC + 1.0f);
    LetTimePass(, 10);

    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());

    // Check that state machine remains in fault state without cycling to init state for long period of time
    for (int i = 0; i < 100; i++)
    {
        LetTimePass(, 10);
        ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
    }

    // Remove fault condition and check transition to init state
    fake_io_ltc6813CellTemps_getMaxTempDegC_returnsForAnyArgs(MAX_CELL_DISCHARGE_TEMP_DEGC - 10.0f);
    LetTimePass(, 10);
    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
}

TEST_F(BmsStateMachineTest, check_precharge_state_transitions_and_air_plus_status)
{
    struct
    {
        bool  air_negative_closes;
        float initial_ts_voltage;
        float precharge_duration;
        bool  expect_precharge_starts;
        bool  expect_precharge_successful;
    } test_params[5] = { {
                             // Precharge doesn't start, AIR- doesn't close
                             .air_negative_closes         = false,
                             .initial_ts_voltage          = 0.0,
                             .precharge_duration          = PRECHARGE_COMPLETION_MS,
                             .expect_precharge_starts     = false,
                             .expect_precharge_successful = false,
                         },
                         {
                             // Precharge doesn't start, TS voltage too high
                             .air_negative_closes         = false,
                             .initial_ts_voltage          = 11.0, // 10V is threshold to precharge
                             .precharge_duration          = PRECHARGE_COMPLETION_MS,
                             .expect_precharge_starts     = false,
                             .expect_precharge_successful = false,
                         },
                         {
                             // Nominal precharge, success
                             .air_negative_closes         = true,
                             .initial_ts_voltage          = 0.0,
                             .precharge_duration          = PRECHARGE_COMPLETION_MS,
                             .expect_precharge_starts     = true,
                             .expect_precharge_successful = true,
                         },
                         {
                             // Fast precharge, fails
                             .air_negative_closes         = true,
                             .initial_ts_voltage          = 0.0,
                             .precharge_duration          = PRECHARGE_COMPLETION_LOWER_BOUND - 30,
                             .expect_precharge_starts     = true,
                             .expect_precharge_successful = false,
                         },
                         {
                             // Slow precharge, fails
                             .air_negative_closes         = true,
                             .initial_ts_voltage          = 0.0,
                             .precharge_duration          = PRECHARGE_COMPLETION_UPPER_BOUND + 30,
                             .expect_precharge_starts     = true,
                             .expect_precharge_successful = false,
                         } };

    for (int i = 0; i < 5; i++)
    {
        TearDown();
        SetUp();

        // Let accumulator startup count expire
        LetTimePass(, 1000);

        // Set initial conditions for precharge
        fake_io_airs_isNegativeClosed_returns(test_params[i].air_negative_closes);
        fake_io_tractiveSystem_getVoltage_returns(test_params[i].initial_ts_voltage);

        if (test_params[i].expect_precharge_starts)
        {
            // Precharge should start
            LetTimePass(, 210U);
            ASSERT_EQ(app_prechargeState_get(), app_stateMachine_getCurrentState());
            ASSERT_EQ(fake_io_airs_closePositive_callCount(), 0);

            // Let precharge duration elapse, confirm still in precharge state and AIR+ open
            LetTimePass(, test_params[i].precharge_duration);
            ASSERT_EQ(app_prechargeState_get(), app_stateMachine_getCurrentState());
            ASSERT_EQ(fake_io_airs_closePositive_callCount(), 0);

            // Set voltage to pack voltage (i.e. voltage successfully rose within duration)
            fake_io_tractiveSystem_getVoltage_returns(
                3.8f * ACCUMULATOR_NUM_SEGMENTS * ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT);

            LetTimePass(, 10);
            if (test_params[i].expect_precharge_successful)
            {
                // Precharge successful, enter drive
                ASSERT_EQ(app_driveState_get(), app_stateMachine_getCurrentState());
                ASSERT_EQ(fake_io_airs_closePositive_callCount(), 1);

                LetTimePass(, 1000);
                ASSERT_EQ(app_driveState_get(), app_stateMachine_getCurrentState());
                ASSERT_EQ(fake_io_airs_closePositive_callCount(), 1);
            }
            else
            {
                // Precharge failed, back to init to try again
                ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
                ASSERT_EQ(fake_io_airs_closePositive_callCount(), 0);

                LetTimePass(, 1000);
                ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
                ASSERT_EQ(fake_io_airs_closePositive_callCount(), 0);
            }
        }
        else
        {
            // Precharge doesn't start, stay in init indefinitely
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
            ASSERT_EQ(fake_io_airs_closePositive_callCount(), 0);

            LetTimePass(, 1000);
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
            ASSERT_EQ(fake_io_airs_closePositive_callCount(), 0);
        }
    }
}

TEST_F(BmsStateMachineTest, perfect_one_percent_soc_decrease)
{
    fake_io_airs_isNegativeClosed_returns(true);
    fake_io_airs_isPositiveClosed_returns(true);
    fake_io_tractiveSystem_getCurrentHighResolution_returns(0.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(0.0f);
    app_soc_resetSocCustomValue(100.0f);

    float soc = app_soc_getMinSocPercent();
    ASSERT_FLOAT_EQ(soc, 100.0f);

    // Allow Timer time to initialize before drawing current
    SetInitialState(app_driveState_get());
    LetTimePass(, 10);

    /* Simulate drawing current.
     * Constant current over 30s span for 1% drop in SOC (0.01)
     * 0.01 = I * dt / (SERIES_ELEMENT_FULL_CHARGE_C)
     *
     * dt = 30s
     *
     * SERIES_ELEMENT_FULL_CHARGE_C = 5.9Ah * 3600 seconds/hour * 3 parallel cells * STATE_OF_HEALTH
     *
     * current (I) = 0.01 * SERIES_ELEMENT_FULL_CHARGE_C / 30
     */

    const float current = -(SERIES_ELEMENT_FULL_CHARGE_C * 0.01f / 30.0f);

    fake_io_tractiveSystem_getCurrentHighResolution_returns(current);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(current);
    app_soc_setPrevCurrent(current);

    LetTimePass(, 30000);

    soc = app_soc_getMinSocPercent();
    ASSERT_FLOAT_EQ(soc, 99.0f);
}

TEST_F(BmsStateMachineTest, ocv_to_soc_lut_test)
{
    // Check that soc saturates at 5.0% lower bound
    float test_voltage = 0.0f;
    float soc          = app_soc_getSocFromOcv(test_voltage);
    float expected_soc = 5.0f; // LUT does not contain SOC values below 5%
    ASSERT_FLOAT_EQ(soc, expected_soc);

    // Check middle of the range
    test_voltage = 4.0f;
    soc          = app_soc_getSocFromOcv(test_voltage);
    expected_soc = 81.5f;
    ASSERT_FLOAT_EQ(soc, expected_soc);

    // Check that SOC saturates at 100.0%
    test_voltage = 5.0f;
    soc          = app_soc_getSocFromOcv(test_voltage);
    expected_soc = 100.0;
    ASSERT_FLOAT_EQ(soc, expected_soc);
}

TEST_F(BmsStateMachineTest, soc_to_ocv_lut_test)
{
    float test_soc     = 0.0f;
    float ocv          = app_soc_getOcvFromSoc(test_soc);
    float expected_ocv = 3.648025f; // LUT does not contain SOC values below 5%
    ASSERT_FLOAT_EQ(ocv, expected_ocv);

    // Check middle of the range
    test_soc     = 68.5f;
    ocv          = app_soc_getOcvFromSoc(test_soc);
    expected_ocv = 3.924725; // LUT does not contain SOC values below 5%
    ASSERT_FLOAT_EQ(ocv, expected_ocv);

    // Check that voltage saturates at value associated with 100.0% soc
    test_soc     = 101.0f;
    ocv          = app_soc_getOcvFromSoc(test_soc);
    expected_ocv = 4.194519f; // LUT does not contain SOC values below 5%
    ASSERT_FLOAT_EQ(ocv, expected_ocv);
}