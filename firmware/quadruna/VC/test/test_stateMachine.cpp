#include "test_vcBaseStateMachineTest.h"

class VCStateMachineTest : public VcBaseStateMachineTest
{
};

TEST_F(VCStateMachineTest, test_SetStateToDrive)
{
    SetStateToDrive();
    LetTimePass(1000);
    EXPECT_EQ(app_driveState_get(), app_stateMachine_getCurrentState());
}

TEST_F(VCStateMachineTest, check_init_state_is_broadcasted_over_can)
{
    SetInitialState(app_initState_get());
    EXPECT_EQ(VC_INIT_STATE, app_canTx_VC_State_get());
}

TEST_F(VCStateMachineTest, check_inverter_on_state_is_broadcasted_over_can)
{
    SetInitialState(app_inverterOnState_get());

    EXPECT_EQ(VC_INVERTER_ON_STATE, app_canTx_VC_State_get());
}

TEST_F(VCStateMachineTest, check_pcm_state_is_broadcasted_over_can)
{
    SetInitialState(app_inverterOnState_get());
    EXPECT_EQ(VC_INVERTER_ON_STATE, app_canTx_VC_State_get());
}

TEST_F(VCStateMachineTest, check_hv_init_state_is_broadcasted_over_can)
{
    SetInitialState(app_hvInitState_get());
    EXPECT_EQ(VC_HV_INIT_STATE, app_canTx_VC_State_get());
}

TEST_F(VCStateMachineTest, check_hv_state_is_broadcasted_over_can)
{
    SetInitialState(app_hvState_get());
    EXPECT_EQ(VC_HV_STATE, app_canTx_VC_State_get());
}

TEST_F(VCStateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(app_driveState_get());
    EXPECT_EQ(VC_DRIVE_STATE, app_canTx_VC_State_get());
}

TEST_F(VCStateMachineTest, check_fault_state_is_broadcasted_over_can)
{
    SetInitialState(app_faultState_get());
    EXPECT_EQ(VC_FAULT_STATE, app_canTx_VC_State_get());
}

TEST_F(VCStateMachineTest, check_drive_warning_state_is_broadcasted_over_can)
{
    SetInitialState(app_driveWarningState_get());
    EXPECT_EQ(VC_DRIVE_WARNING_STATE, app_canTx_VC_State_get());
}

TEST_F(VCStateMachineTest, check_init_to_inverter_on_transition)
{
    SetInitialState(app_initState_get());

    // Make sure we stay in init state if AIR- is open
    LetTimePass(100);
    EXPECT_EQ(app_initState_get(), app_stateMachine_getCurrentState());

    app_canRx_BMS_AirNegative_update(CONTACTOR_STATE_CLOSED);
    EXPECT_EQ(app_canRx_BMS_AirNegative_get(), CONTACTOR_STATE_CLOSED);
    LetTimePass(1000);
    EXPECT_EQ(app_inverterOnState_get(), app_stateMachine_getCurrentState());
}

TEST_F(VCStateMachineTest, check_inverter_on_to_pcm_transition)
{
    app_canRx_BMS_AirNegative_update(CONTACTOR_STATE_CLOSED);
    SetInitialState(app_inverterOnState_get());

    app_canRx_BMS_State_update(BMS_DRIVE_STATE);
    app_canRx_INVL_VsmState_update(INVERTER_VSM_READY_STATE);
    app_canRx_INVR_VsmState_update(INVERTER_VSM_READY_STATE);

    LetTimePass(10);
    EXPECT_EQ(app_pcmState_get(), app_stateMachine_getCurrentState());
}

TEST_F(VCStateMachineTest, check_hv_to_drive_state_transition)
{
    app_canRx_BMS_AirNegative_update(CONTACTOR_STATE_CLOSED);
    app_canRx_BMS_State_update(BMS_DRIVE_STATE);

    // Stay in hv until start switch is on and brake is actuated
    SetInitialState(app_hvState_get());
    LetTimePass(1000);
    EXPECT_EQ(app_hvState_get(), app_stateMachine_getCurrentState());

    // Once start switch is on and brake is actuated stay in drive
    app_canRx_CRIT_StartSwitch_update(SWITCH_ON);
    app_canRx_FSM_BrakeActuated_update(true);
    LetTimePass(1000);

    EXPECT_EQ(app_driveState_get(), app_stateMachine_getCurrentState());
}

TEST_F(VCStateMachineTest, check_drive_to_hv_state_start_switch_off)
{
    SetStateToDrive();

    app_canRx_CRIT_StartSwitch_update(SWITCH_OFF);
    LetTimePass(1000);
    EXPECT_EQ(app_hvState_get(), app_stateMachine_getCurrentState());
}

TEST_F(VCStateMachineTest, check_drive_to_init_state_when_hv_lost)
{
    SetStateToDrive();

    app_canRx_BMS_State_update(BMS_INIT_STATE);
    LetTimePass(10);
    EXPECT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
}

TEST_F(VCStateMachineTest, check_drive_to_fault_state_when_bms_fault)
{
    SetStateToDrive();

    app_canRx_BMS_Fault_StateMachine_update(true);
    LetTimePass(100);
    EXPECT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());
}

TEST_F(VCStateMachineTest, check_drive_to_fault_to_init_state)
{
    SetStateToDrive();

    // Go into fault state, caused by BMS fault
    app_canRx_BMS_Fault_StateMachine_update(true);
    LetTimePass(100);
    EXPECT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());

    // clear all faults and go back into init state
    app_canRx_BMS_Fault_StateMachine_update(false);
    LetTimePass(10);
    EXPECT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
}

TEST_F(VCStateMachineTest, check_hv_to_init_state_when_hv_lost)
{
    app_canRx_BMS_State_update(BMS_DRIVE_STATE);
    app_canRx_BMS_AirNegative_update(CONTACTOR_STATE_CLOSED);

    SetInitialState(app_hvState_get());

    app_canRx_BMS_State_update(BMS_INIT_STATE);
    LetTimePass(10);
    EXPECT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
}

TEST_F(VCStateMachineTest, check_hv_init_to_init_state_when_hv_lost)
{
    app_canRx_BMS_State_update(BMS_DRIVE_STATE);
    app_canRx_BMS_AirNegative_update(CONTACTOR_STATE_CLOSED);

    SetInitialState(app_hvInitState_get());

    app_canRx_BMS_State_update(BMS_INIT_STATE);
    LetTimePass(10);
    EXPECT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
}

TEST_F(VCStateMachineTest, check_pcm_to_init_state_when_hv_lost)
{
    app_canRx_BMS_State_update(BMS_DRIVE_STATE);
    app_canRx_BMS_AirNegative_update(CONTACTOR_STATE_CLOSED);

    SetInitialState(app_pcmState_get());

    app_canRx_BMS_State_update(BMS_INIT_STATE);
    LetTimePass(10);
    EXPECT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
}

TEST_F(VCStateMachineTest, check_if_buzzer_stays_on_for_two_seconds_only_after_entering_drive_state)
{
    for (auto &state : GetAllStates())
    {
        VcBaseStateMachineTest::SetUp();
        SetInitialState(state);
        app_heartbeatMonitor_clearFaults();

        if (app_canTx_VC_State_get() == VC_DRIVE_STATE)
        {
            // Set the CRIT start switch to on, and the BMS to drive state, to prevent state transitions in
            // the drive state.
            app_canRx_CRIT_StartSwitch_update(SWITCH_ON);
            app_canRx_BMS_State_update(BMS_DRIVE_STATE);
            EXPECT_TRUE(app_canTx_VC_BuzzerOn_get());

            LetTimePass(BUZZER_ON_DURATION_MS);
            EXPECT_TRUE(app_canTx_VC_BuzzerOn_get());

            // Check that the buzzer has been turned off after waiting for
            // BUZZER_ON_DURATION_MS.
            LetTimePass(10);
            EXPECT_FALSE(app_canTx_VC_BuzzerOn_get());

            // Check that the buzzer stays off indefinitely.
            LetTimePass(100);
            EXPECT_FALSE(app_canTx_VC_BuzzerOn_get());
        }
        else
        {
            // Check that the buzzer is not turned on in other states.
            EXPECT_FALSE(app_canTx_VC_BuzzerOn_get());
        }
    }
}

TEST_F(VCStateMachineTest, no_torque_requests_when_accelerator_pedal_is_not_pressed)
{
    SetStateToDrive();

    // Set the CRIT start switch to on, and the BMS to drive state, to prevent state transitions in
    // the drive state.
    app_canRx_CRIT_StartSwitch_update(SWITCH_ON);
    app_canRx_BMS_State_update(BMS_DRIVE_STATE);

    // Check that no torque requests are sent when the accelerator pedal is not
    // pressed
    LetTimePass(10);
    EXPECT_FLOAT_EQ(0.0f, app_canTx_VC_LeftInverterTorqueCommand_get());
    EXPECT_FLOAT_EQ(0.0f, app_canTx_VC_RightInverterTorqueCommand_get());
}

TEST_F(VCStateMachineTest, exit_drive_state_on_left_inverter_fault)
{
    SetStateToDrive();

    app_canRx_INVL_VsmState_update(INVERTER_VSM_BLINK_FAULT_CODE_STATE);
    LetTimePass(10);
    EXPECT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());

    app_canRx_INVL_VsmState_update(INVERTER_VSM_START_STATE);
    LetTimePass(10);
    EXPECT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
}

TEST_F(VCStateMachineTest, exit_drive_state_on_right_inverter_fault)
{
    SetStateToDrive();

    app_canRx_INVR_VsmState_update(INVERTER_VSM_BLINK_FAULT_CODE_STATE);
    LetTimePass(10);
    EXPECT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());

    app_canRx_INVR_VsmState_update(INVERTER_VSM_START_STATE);
    LetTimePass(10);
    EXPECT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
}

TEST_F(VCStateMachineTest, disable_inverters_in_init_state)
{
    SetStateToDrive();
    app_canTx_VC_LeftInverterTorqueCommand_set(1.0F);
    app_canTx_VC_RightInverterTorqueCommand_set(1.0F);
    app_canTx_VC_LeftInverterEnable_set(true);
    app_canTx_VC_RightInverterEnable_set(true);

    EXPECT_FLOAT_EQ(1.0f, app_canTx_VC_LeftInverterTorqueCommand_get());
    EXPECT_FLOAT_EQ(1.0f, app_canTx_VC_RightInverterTorqueCommand_get());
    EXPECT_FLOAT_EQ(true, app_canTx_VC_LeftInverterEnable_get());
    EXPECT_FLOAT_EQ(true, app_canTx_VC_RightInverterEnable_get());

    app_canRx_BMS_State_update(BMS_INIT_STATE);

    // Now tick the state machine and check torque request gets zeroed on transition to init.
    LetTimePass(10);
    EXPECT_FLOAT_EQ(0.0f, app_canTx_VC_LeftInverterTorqueCommand_get());
    EXPECT_FLOAT_EQ(0.0f, app_canTx_VC_RightInverterTorqueCommand_get());
    EXPECT_FLOAT_EQ(false, app_canTx_VC_LeftInverterEnable_get());
    EXPECT_FLOAT_EQ(false, app_canTx_VC_RightInverterEnable_get());
}

TEST_F(VCStateMachineTest, exit_drive_state_on_fsm_fault)
{
    SetStateToDrive();
    app_canRx_FSM_Fault_PappsOCSC_update(true);
    LetTimePass(10);
    EXPECT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());

    app_canRx_FSM_Fault_PappsOCSC_update(false);
    LetTimePass(10);
    EXPECT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
}

TEST_F(VCStateMachineTest, exit_drive_state_on_CRIT_fault)
{
    SetStateToDrive();
    app_canRx_CRIT_Fault_MissingBMSHeartbeat_update(true);
    LetTimePass(10);
    EXPECT_EQ(app_faultState_get(), app_stateMachine_getCurrentState());

    app_canRx_CRIT_Fault_MissingBMSHeartbeat_update(false);
    LetTimePass(10);
    EXPECT_EQ(app_initState_get(), app_stateMachine_getCurrentState());
}
