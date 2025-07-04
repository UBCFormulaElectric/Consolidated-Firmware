
#include "fake_io_faultLatch.hpp"
#include "test_BMSBase.hpp"

extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_stateMachine.h"
#include "app_tractiveSystem.h"
#include "app_precharge.h"
#include "app_canUtils.h"
#include "io_faultLatch.h"
}

class BmsStateMachineTest : public BMSBaseTest
{
};

TEST_F(BmsStateMachineTest, check_init_state_is_broadcasted_over_can)
{
    SetInitialState(app_initState_get());
    EXPECT_EQ(BMS_INIT_STATE, app_canTx_BMS_State_get());
}

TEST_F(BmsStateMachineTest, check_precharge_drive_state_is_broadcasted_over_can)
{
    SetInitialState(app_prechargeDriveState_get());
    EXPECT_EQ(BMS_PRECHARGE_DRIVE_STATE, app_canTx_BMS_State_get());
}

TEST_F(BmsStateMachineTest, check_precharge_charge_state_is_broadcasted_over_can)
{
    SetInitialState(app_prechargeChargeState_get());
    EXPECT_EQ(BMS_PRECHARGE_CHARGE_STATE, app_canTx_BMS_State_get());
}

TEST_F(BmsStateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(app_driveState_get());
    EXPECT_EQ(BMS_DRIVE_STATE, app_canTx_BMS_State_get());
}

TEST_F(BmsStateMachineTest, check_fault_state_is_broadcasted_over_can)
{
    SetInitialState(app_faultState_get());
    EXPECT_EQ(BMS_FAULT_STATE, app_canTx_BMS_State_get());
}

TEST_F(BmsStateMachineTest, check_charge_state_is_broadcasted_over_can)
{
    SetInitialState(app_chargeState_get());
    EXPECT_EQ(BMS_CHARGE_STATE, app_canTx_BMS_State_get());
}

TEST_F(BmsStateMachineTest, check_imd_frequency_is_broadcasted_over_can_in_all_states)
{
    float fake_frequency = 0.0f;

    for (const auto &state : GetAllStates())
    {
        SetInitialState(state);
        fake_io_imd_getFrequency_returns(fake_frequency);
        LetTimePass(10);

        EXPECT_EQ(fake_frequency, app_canTx_BMS_ImdFrequency_get());

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

        LetTimePass(10);

        EXPECT_EQ(fake_duty_cycle, app_canTx_BMS_ImdDutyCycle_get());

        // To avoid false positives, we use a different frequency each time
        fake_duty_cycle++;
    }
}

TEST_F(BmsStateMachineTest, check_imd_insulation_resistance_10hz_is_broadcasted_over_can_in_all_states)
{
    for (const auto &state : GetAllStates())
    {
        SetInitialState(state);
        SetImdCondition(IMD_CONDITION_NORMAL);

        // Test an arbitrarily chosen valid resistance
        fake_io_imd_getDutyCycle_returns(50.0f);
        LetTimePass(10);

        EXPECT_EQ(IMD_CONDITION_NORMAL, app_canTx_BMS_ImdCondition_get());
        EXPECT_EQ(true, app_canTx_BMS_ImdValidDutyCycle_get());
        EXPECT_EQ(1200, app_canTx_BMS_ImdInsulationMeasurementDcp10Hz_get());

        // Test an arbitrarily chosen invalid resistance
        fake_io_imd_getDutyCycle_returns(0.0f);
        LetTimePass(10);
        EXPECT_EQ(IMD_CONDITION_NORMAL, app_canTx_BMS_ImdCondition_get());
        EXPECT_EQ(false, app_canTx_BMS_ImdValidDutyCycle_get());
    }
}

TEST_F(BmsStateMachineTest, check_imd_insulation_resistance_20hz_is_broadcasted_over_can_in_all_states)
{
    for (const auto &state : GetAllStates())
    {
        SetInitialState(state);
        SetImdCondition(IMD_CONDITION_UNDERVOLTAGE_DETECTED);

        // Test an arbitrarily chosen valid resistance
        fake_io_imd_getDutyCycle_returns(50.0f);
        LetTimePass(10);

        EXPECT_EQ(IMD_CONDITION_UNDERVOLTAGE_DETECTED, app_canTx_BMS_ImdCondition_get());
        EXPECT_EQ(true, app_canTx_BMS_ImdValidDutyCycle_get());
        EXPECT_EQ(1200, app_canTx_BMS_ImdInsulationMeasurementDcp20Hz_get());

        // Test an arbitrarily chosen invalid resistance
        fake_io_imd_getDutyCycle_returns(0.0f);
        LetTimePass(10);
        EXPECT_EQ(IMD_CONDITION_UNDERVOLTAGE_DETECTED, app_canTx_BMS_ImdCondition_get());
        EXPECT_EQ(false, app_canTx_BMS_ImdValidDutyCycle_get());
    }
}

TEST_F(BmsStateMachineTest, check_imd_speed_start_status_30hz_is_broadcasted_over_can_in_all_states)
{
    for (const auto &state : GetAllStates())
    {
        SetInitialState(state);
        SetImdCondition(IMD_CONDITION_SST);

        // Test an arbitrarily chosen SST_GOOD
        fake_io_imd_getDutyCycle_returns(7.5f);

        LetTimePass(10);
        EXPECT_EQ(IMD_CONDITION_SST, app_canTx_BMS_ImdCondition_get());
        EXPECT_EQ(true, app_canTx_BMS_ImdValidDutyCycle_get());
        EXPECT_EQ(SST_GOOD, app_canTx_BMS_ImdSpeedStartStatus30Hz_get());

        // Test an arbitrarily chosen SST_BAD
        fake_io_imd_getDutyCycle_returns(92.5f);

        LetTimePass(10);
        EXPECT_EQ(IMD_CONDITION_SST, app_canTx_BMS_ImdCondition_get());
        EXPECT_EQ(true, app_canTx_BMS_ImdValidDutyCycle_get());
        EXPECT_EQ(SST_BAD, app_canTx_BMS_ImdSpeedStartStatus30Hz_get());

        // Test an arbitrarily chosen invalid SST status
        fake_io_imd_getDutyCycle_returns(0.0f);

        LetTimePass(10);
        EXPECT_EQ(IMD_CONDITION_SST, app_canTx_BMS_ImdCondition_get());
        EXPECT_EQ(false, app_canTx_BMS_ImdValidDutyCycle_get());
    }
}

TEST_F(BmsStateMachineTest, check_imd_seconds_since_power_on_is_broadcasted_over_can_in_all_states)
{
    for (const auto &state : GetAllStates())
    {
        SetInitialState(state);
        fake_io_imd_getTimeSincePowerOn_returns(123);
        LetTimePass(10);
        EXPECT_EQ(123, app_canTx_BMS_ImdTimeSincePowerOn_get());
    }
}

// TODO: Detect Elcon charger.
// TEST_F(BmsStateMachineTest, charger_connection_status_in_all_states)
// {
//     for (const auto &state : GetAllStates())
//     {
//         SetInitialState(state);

//         app_canRx_BRUSA_IsConnected_update(true);
//         LetTimePass(1000);
//         EXPECT_EQ(true, app_canTx_BMS_ChargerConnected_get());

//         app_canRx_BRUSA_IsConnected_update(false);
//         LetTimePass(1000);
//         EXPECT_EQ(false, app_canTx_BMS_ChargerConnected_get());
//     }
// }

TEST_F(BmsStateMachineTest, check_bms_ok_is_broadcasted_over_can_in_all_states)
{
    // Enable BMS_OK
    fake_io_faultLatch_getCurrentStatus_returnsForArgs(&bms_ok_latch, true);

    for (const auto &state : GetAllStates())
    {
        SetInitialState(state);

        // Make sure the state machine sets the CAN signal for BMS_OK
        LetTimePass(10);
        ASSERT_EQ(true, app_canTx_BMS_BmsCurrentlyOk_get());

        // Reset the CAN signal for BMS_OK
        app_canTx_BMS_BmsCurrentlyOk_set(false);
    }
}

TEST_F(BmsStateMachineTest, check_imd_ok_is_broadcasted_over_can_in_all_states)
{
    // Enable IMD_OK
    fake_io_faultLatch_getCurrentStatus_returnsForArgs(&imd_ok_latch, true);

    for (const auto &state : GetAllStates())
    {
        SetInitialState(state);

        // Make sure the state machine sets the CAN signal for IMD_OK
        LetTimePass(10);
        ASSERT_EQ(true, app_canTx_BMS_ImdCurrentlyOk_get());

        // Reset the CAN signal for IMD_OK
        app_canTx_BMS_ImdCurrentlyOk_set(false);
    }
}

TEST_F(BmsStateMachineTest, check_bspd_ok_is_broadcasted_over_can_in_all_states)
{
    // Enable BSPD_OK
    fake_io_faultLatch_getCurrentStatus_returnsForArgs(&bspd_ok_latch, true);

    for (const auto &state : GetAllStates())
    {
        SetInitialState(state);

        // Make sure the state machine sets the CAN signal for BSPD_OK
        LetTimePass(10);
        ASSERT_EQ(true, app_canTx_BMS_BspdCurrentlyOk_get());

        // Reset the CAN signal for BSPD_OK
        app_canTx_BMS_BspdCurrentlyOk_set(false);
    }
}

// TODO: Detect Elcon charger.
// TEST_F(BmsStateMachineTest, stops_charging_and_faults_if_charger_disconnects_in_charge_state)
// {
//     SetInitialState(app_chargeState_get());
//     fake_io_irs_isNegativeClosed_returns(true);

//     // Set the current values to above the threshold for charging to stop (charging should continue)
//     fake_io_tractiveSystem_getCurrentHighResolution_returns(1.0f);
//     fake_io_tractiveSystem_getCurrentLowResolution_returns(1.0f);

//     // Simulate situation with charger present and user indicate to start charging
//     app_canRx_BRUSA_IsConnected_update(false);
//     app_canRx_Debug_StartCharging_update(true);

//     LetTimePass(1000);

//     // Checks if a CAN message was sent to indicate charger was disconnected unexpectedly
//     ASSERT_EQ(true, app_canAlerts_BMS_Fault_ChargerDisconnectedDuringCharge_get());
//     ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
// }

TEST_F(BmsStateMachineTest, check_airs_can_signals_for_all_states)
{
    for (const auto &state : GetAllStates())
    {
        SetInitialState(state);

        fake_io_irs_isNegativeClosed_returns(false);
        fake_io_irs_isPositiveClosed_returns(false);

        LetTimePass(10);
        ASSERT_EQ(CONTACTOR_STATE_OPEN, app_canTx_BMS_IrNegative_get());
        ASSERT_EQ(CONTACTOR_STATE_OPEN, app_canTx_BMS_IrPositive_get());

        fake_io_irs_isNegativeClosed_returns(false);
        fake_io_irs_isPositiveClosed_returns(true);

        LetTimePass(10);
        ASSERT_EQ(CONTACTOR_STATE_OPEN, app_canTx_BMS_IrNegative_get());
        ASSERT_EQ(CONTACTOR_STATE_CLOSED, app_canTx_BMS_IrPositive_get());

        fake_io_irs_isNegativeClosed_returns(true);
        fake_io_irs_isPositiveClosed_returns(false);

        LetTimePass(10);
        ASSERT_EQ(CONTACTOR_STATE_CLOSED, app_canTx_BMS_IrNegative_get());
        ASSERT_EQ(CONTACTOR_STATE_OPEN, app_canTx_BMS_IrPositive_get());

        fake_io_irs_isNegativeClosed_returns(true);
        fake_io_irs_isPositiveClosed_returns(true);

        LetTimePass(10);
        ASSERT_EQ(CONTACTOR_STATE_CLOSED, app_canTx_BMS_IrNegative_get());
        ASSERT_EQ(CONTACTOR_STATE_CLOSED, app_canTx_BMS_IrPositive_get());
    }
}

TEST_F(BmsStateMachineTest, check_contactors_open_in_fault_state)
{
    fake_io_irs_openPositive_reset();

    SetInitialState(app_faultState_get());
    ASSERT_EQ(fake_io_irs_openPositive_callCount(), 1);
}

TEST_F(BmsStateMachineTest, check_state_transition_from_fault_to_init_with_no_faults_set)
{
    // Assume no AIR shutdown faults have been set
    SetInitialState(app_faultState_get());

    fake_io_faultLatch_getLatchedStatus_returnsForArgs(&bms_ok_latch, false);
    LetTimePass(1000);
    ASSERT_EQ(BMS_FAULT_STATE, app_canTx_BMS_State_get());

    fake_io_faultLatch_getLatchedStatus_returnsForArgs(&bms_ok_latch, true);
    LetTimePass(1000);
    ASSERT_EQ(BMS_INIT_STATE, app_canTx_BMS_State_get());
}

// TODO: Detect Elcon charger.
// TEST_F(BmsStateMachineTest, charger_connected_no_can_msg_init_state)
// {
//     SetInitialState(app_initState_get());
//     fake_io_irs_isNegativeClosed_returns(true);

//     // Without the CAN message to start charging, will remain in init state when charger is connected
//     app_canRx_BRUSA_IsConnected_update(true);

//     LetTimePass(20);

//     ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
// }

TEST_F(BmsStateMachineTest, charger_connected_successful_precharge_stays)
{
    SetInitialState(app_initState_get());
    fake_io_irs_isNegativeClosed_returns(true);

    // Set the current values to above the threshold for charging to stop (charging should continue)
    fake_io_tractiveSystem_getCurrentHighResolution_returns(1.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(1.0f);

    // Simulate situation with charger present and user indicate to start charging
    app_canRx_Debug_StartCharging_update(true);

    // Allow BMS time to go through Init state
    LetTimePass(10);
    fake_io_tractiveSystem_getVoltage_returns(550.0f);

    // Pause for slightly longer to allow pre-charge
    LetTimePass(210);

    printf("%s", app_stateMachine_getCurrentState()->name);
    ASSERT_EQ(app_chargeState_get(), app_stateMachine_getCurrentState());
}

TEST_F(BmsStateMachineTest, keeps_charging_with_no_interrupts)
{
    SetInitialState(app_chargeState_get());
    fake_io_irs_isNegativeClosed_returns(true);

    // Set the current values to above the threshold for charging to stop (charging should continue)
    fake_io_tractiveSystem_getCurrentHighResolution_returns(1.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(1.0f);

    // Simulate situation with charger present and user indicate to start charging
    app_canRx_Debug_StartCharging_update(true);

    LetTimePass(100);

    ASSERT_EQ(app_chargeState_get(), app_stateMachine_getCurrentState());
}

TEST_F(BmsStateMachineTest, stops_charging_after_false_charging_msg)
{
    SetInitialState(app_chargeState_get());
    fake_io_irs_isNegativeClosed_returns(true);

    // Set the current values to above the threshold for charging to stop (charging should continue)
    fake_io_tractiveSystem_getCurrentHighResolution_returns(1.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(1.0f);

    // Simulate situation with charger present and user indicate to start charging
    app_canRx_Debug_StartCharging_update(false);

    LetTimePass(1000);
    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
}

TEST_F(BmsStateMachineTest, fault_from_charger_fault)
{
    SetInitialState(app_chargeState_get());
    fake_io_irs_isNegativeClosed_returns(true);

    // Set the current values to above the threshold for charging to stop (charging should continue)
    fake_io_tractiveSystem_getCurrentHighResolution_returns(1.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(1.0f);

    // Simulate situation with charger present and user indicate to start charging
    app_canRx_Elcon_ChargerOverTemperature_update(true);
    app_canRx_Debug_StartCharging_update(true);

    // Charger faults are debounced for 3s
    LetTimePass(3010);
    const State *currentState = app_stateMachine_getCurrentState();

    ASSERT_EQ(app_initState_get(), currentState);
}

TEST_F(BmsStateMachineTest, check_remains_in_fault_state_until_fault_cleared_then_transitions_to_init)
{
    SetInitialState(app_initState_get());

    // Let accumulator startup count expire
    LetTimePass(1000);

    // Set TS current negative to trigger discharging condition in tempertature check
    fake_io_tractiveSystem_getCurrentHighResolution_returns(-10.0f);
    fake_io_tractiveSystem_getCurrentLowResolution_returns(-10.0f);

    // Simulate over-voltage fault in drive state
    fakes::segments::SetCellVoltage(0, 0, MAX_CELL_VOLTAGE_FAULT_V + 0.1);
    LetTimePass(LTC_CONVERSION_PERIOD_MS + OVER_VOLTAGE_DEBOUNCE_FAULT_MS + 10);

    ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
    fake_io_faultLatch_getLatchedStatus_returnsForArgs(&bms_ok_latch, false);

    // Check that state machine remains in fault state without cycling to init state for long period of time
    for (int i = 0; i < 100; i++)
    {
        LetTimePass(10);
        ASSERT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
    }

    // Remove fault condition and check transition to init state
    fakes::segments::SetCellVoltage(0, 0, MAX_CELL_VOLTAGE_FAULT_V - 0.1);
    fake_io_faultLatch_getLatchedStatus_returnsForArgs(&bms_ok_latch, true);

    LetTimePass(LTC_CONVERSION_PERIOD_MS + 10);
    ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
}

TEST_F(BmsStateMachineTest, check_precharge_state_transitions_and_air_plus_status)
{
    struct
    {
        bool    air_negative_closes;
        float   initial_ts_voltage;
        VCState vc_state;
        float   precharge_duration;
        bool    expect_precharge_starts;
        bool    expect_precharge_successful;
    } test_params[5] = { {
                             // Precharge doesn't start, AIR- doesn't close
                             .air_negative_closes         = false,
                             .initial_ts_voltage          = 0.0,
                             .vc_state                    = VC_BMS_ON_STATE,
                             .precharge_duration          = PRECHARGE_COMPLETION_MS,
                             .expect_precharge_starts     = false,
                             .expect_precharge_successful = false,
                         },
                         {
                             // Precharge doesn't start, TS voltage too high
                             .air_negative_closes         = false,
                             .initial_ts_voltage          = 11.0, // 10V is threshold to precharge
                             .vc_state                    = VC_BMS_ON_STATE,
                             .precharge_duration          = PRECHARGE_COMPLETION_MS,
                             .expect_precharge_starts     = false,
                             .expect_precharge_successful = false,
                         },
                         {
                             // Nominal precharge, success
                             .air_negative_closes         = true,
                             .initial_ts_voltage          = 0.0,
                             .vc_state                    = VC_BMS_ON_STATE,
                             .precharge_duration          = PRECHARGE_COMPLETION_MS,
                             .expect_precharge_starts     = true,
                             .expect_precharge_successful = true,
                         },
                         {
                             // Fast precharge, fails
                             .air_negative_closes         = true,
                             .initial_ts_voltage          = 0.0,
                             .vc_state                    = VC_BMS_ON_STATE,
                             .precharge_duration          = PRECHARGE_COMPLETION_LOWER_BOUND - 30,
                             .expect_precharge_starts     = true,
                             .expect_precharge_successful = false,
                         },
                         {
                             // Slow precharge, fails
                             .air_negative_closes = true,
                             .initial_ts_voltage  = 0.0,
                             .vc_state            = VC_BMS_ON_STATE,
                             .precharge_duration =
                                 PRECHARGE_COMPLETION_UPPER_BOUND + 20U, // Allow inverter on state to complete again
                             .expect_precharge_starts     = true,
                             .expect_precharge_successful = false,
                         } };

    for (int i = 0; i < 5; i++)
    {
        TearDown();
        SetUp();

        // Let accumulator startup count expire
        LetTimePass(1000);

        // Set initial conditions for precharge
        fake_io_irs_isNegativeClosed_returns(test_params[i].air_negative_closes);
        fake_io_tractiveSystem_getVoltage_returns(test_params[i].initial_ts_voltage);
        app_canRx_VC_State_update(test_params[i].vc_state);

        if (test_params[i].expect_precharge_starts)
        {
            // Precharge should start
            LetTimePass(10U);
            ASSERT_EQ(app_prechargeDriveState_get(), app_stateMachine_getCurrentState());
            ASSERT_EQ(fake_io_irs_closePositive_callCount(), 0);

            // Let precharge duration elapse, confirm still in precharge state and AIR+ open
            LetTimePass(test_params[i].precharge_duration);
            ASSERT_EQ(app_prechargeDriveState_get(), app_stateMachine_getCurrentState());
            ASSERT_EQ(fake_io_irs_closePositive_callCount(), 0);

            // Set voltage to pack voltage (i.e. voltage successfully rose within duration)
            fake_io_tractiveSystem_getVoltage_returns(3.8 * NUM_SEGMENTS * CELLS_PER_SEGMENT);
            LetTimePass(10);

            if (test_params[i].expect_precharge_successful)
            {
                // Precharge successful, enter drive
                ASSERT_EQ(app_driveState_get(), app_stateMachine_getCurrentState());
                ASSERT_EQ(fake_io_irs_closePositive_callCount(), 1);

                LetTimePass(1000);
                ASSERT_EQ(app_driveState_get(), app_stateMachine_getCurrentState());
                ASSERT_EQ(fake_io_irs_closePositive_callCount(), 1);
            }
            else
            {
                // Precharge failed, back to init to try again
                ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
                ASSERT_EQ(fake_io_irs_closePositive_callCount(), 0);

                LetTimePass(1000);
                ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
                ASSERT_EQ(fake_io_irs_closePositive_callCount(), 0);
            }
        }
        else
        {
            // Precharge doesn't start, stay in init indefinitely
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
            ASSERT_EQ(fake_io_irs_closePositive_callCount(), 0);

            LetTimePass(1000);
            ASSERT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
            ASSERT_EQ(fake_io_irs_closePositive_callCount(), 0);
        }
    }
}
