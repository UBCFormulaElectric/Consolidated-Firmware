#include "test_vcBaseStateMachineTest.h"

class VCStateMachineTest : public VcBaseStateMachineTest
{
  protected:
    void TestFaultBlocksDrive(std::function<void(void)> set_fault, std::function<void(void)> clear_fault)
    {
        SetInitialState(app_driveState_get());

        // Set the CRIT start switch to on, and the BMS to drive state, to prevent state transitions in
        // the drive state.
        app_canRx_CRIT_StartSwitch_update(SWITCH_ON);
        app_canRx_BMS_State_update(BMS_DRIVE_STATE);
        LetTimePass(10);
        EXPECT_EQ(VC_DRIVE_STATE, app_canTx_VC_State_get());

        // Introduce fault, expect transition to init state.
        set_fault();
        LetTimePass(10);
        EXPECT_EQ(VC_INIT_STATE, app_canTx_VC_State_get());

        // Confirm we don't allow a transition back to drive until the fault clears.
        LetTimePass(1000);
        EXPECT_EQ(VC_INIT_STATE, app_canTx_VC_State_get());

        // Clear fault and observe a transition back to drive, when the drive conditions are met.
        clear_fault();

        app_canRx_FSM_BrakeActuated_update(true);
        app_canRx_CRIT_StartSwitch_update(SWITCH_OFF);
        //had to increase time from 10 to 20 to allow transition from inverterOnState to drivestate 
        LetTimePass(20);
        app_canRx_CRIT_StartSwitch_update(SWITCH_ON);
        LetTimePass(20);
        EXPECT_EQ(VC_DRIVE_STATE, app_canTx_VC_State_get());
    }
};

TEST_F(VCStateMachineTest, check_init_transitions_to_drive_if_conditions_met_and_start_switch_pulled_up)
{
    app_canRx_CRIT_StartSwitch_update(SWITCH_OFF);
    app_canRx_CRIT_StartSwitch_update(SWITCH_ON);
    // Pull start switch down and back up, expect no transition as inverters are not on/BMS is not in drive state
    EXPECT_EQ(app_initState_get(), app_stateMachine_getCurrentState());

    // Transition BMS to drive state, expect transition to inverter_state as power manage is in drive state 
    app_canRx_BMS_State_update(BMS_DRIVE_STATE);
    LetTimePass(10);
    EXPECT_EQ(app_inverterOnState_get(), app_stateMachine_getCurrentState());

    // Actuate brake pedal
    app_canRx_FSM_BrakeActuated_update(true);
    LetTimePass(10);
    EXPECT_EQ(app_inverterOnState_get(), app_stateMachine_getCurrentState());

    // Pull start switch down and back up, expect init->drive transition
    app_canRx_CRIT_StartSwitch_update(SWITCH_OFF);
    LetTimePass(10);
    app_canRx_CRIT_StartSwitch_update(SWITCH_ON);
    LetTimePass(10);
    EXPECT_EQ(VC_DRIVE_STATE, app_canTx_VC_State_get());
}

TEST_F(VCStateMachineTest, check_init_state_is_broadcasted_over_can)
{
    SetInitialState(app_initState_get());
    EXPECT_EQ(VC_INIT_STATE, app_canTx_VC_State_get());
}

TEST_F(VCStateMachineTest, check_state_transition_from_init_to_inverter_on)
{
    SetInitialState(app_initState_get());
    app_canRx_BMS_State_update(BMS_INVERTER_ON_STATE);
    LetTimePass(1000);
    EXPECT_EQ(VC_INVERTER_ON_STATE, app_canTx_VC_State_get());
    EXPECT_EQ(app_powerManager_getState(), POWER_MANAGER_DRIVE);
}

// TODO: add inverter_on to drive state test
TEST_F(VCStateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(app_driveState_get());

    EXPECT_EQ(VC_DRIVE_STATE, app_canTx_VC_State_get());
}

TEST_F(VCStateMachineTest, disable_inverters_in_init_state)
{
    SetInitialState(app_initState_get());
    app_canRx_BMS_State_update(BMS_INVERTER_ON_STATE);
    LetTimePass(1000);
    //Transitioning from init state to inverter on state as inverters have been turned on 
    EXPECT_EQ(VC_INVERTER_ON_STATE, app_canTx_VC_State_get());
    EXPECT_EQ(app_powerManager_getState(), POWER_MANAGER_DRIVE);
    LetTimePass(1000);
    // Start in drive with a non-zero torque request to prevent false positive.
    SetInitialState(app_driveState_get());
    app_canTx_VC_LeftInverterTorqueCommand_set(1.0f);
    app_canTx_VC_RightInverterTorqueCommand_set(1.0f);
    app_canTx_VC_LeftInverterEnable_set(true);
    app_canTx_VC_RightInverterEnable_set(true);

    EXPECT_FLOAT_EQ(1.0f, app_canTx_VC_LeftInverterTorqueCommand_get());
    EXPECT_FLOAT_EQ(1.0f, app_canTx_VC_RightInverterTorqueCommand_get());
    EXPECT_FLOAT_EQ(true, app_canTx_VC_LeftInverterEnable_get());
    EXPECT_FLOAT_EQ(true, app_canTx_VC_RightInverterEnable_get());

    // Flip start switch down causing transition to init state.
    app_canRx_CRIT_StartSwitch_update(SWITCH_OFF);

    // Now tick the state machine and check torque request gets zeroed on transition to init.
    LetTimePass(10);
    EXPECT_FLOAT_EQ(0.0f, app_canTx_VC_LeftInverterTorqueCommand_get());
    EXPECT_FLOAT_EQ(0.0f, app_canTx_VC_RightInverterTorqueCommand_get());
    EXPECT_FLOAT_EQ(false, app_canTx_VC_LeftInverterEnable_get());
    EXPECT_FLOAT_EQ(false, app_canTx_VC_RightInverterEnable_get());
}

TEST_F(VCStateMachineTest, start_switch_off_transitions_drive_state_to_init_state)
{
    SetInitialState(app_driveState_get());
    app_canRx_CRIT_StartSwitch_update(SWITCH_OFF);
    LetTimePass(10);

    ASSERT_EQ(VC_INIT_STATE, app_canTx_VC_State_get());
}

TEST_F(VCStateMachineTest, check_if_buzzer_stays_on_for_two_seconds_only_after_entering_drive_state)
{
    for (auto &state : GetAllStates())
    {
        BaseStateMachineTest::SetUp();
        SetInitialState(state);

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
    SetInitialState(app_driveState_get());

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

TEST_F(VCStateMachineTest, drive_to_init_state_on_left_inverter_fault)
{
    auto set_fault   = []() { app_canRx_INVR_VsmState_update(INVERTER_VSM_BLINK_FAULT_CODE_STATE); };
    auto clear_fault = []() { app_canRx_INVR_VsmState_update(INVERTER_VSM_START_STATE); };

    TestFaultBlocksDrive(set_fault, clear_fault);
}

TEST_F(VCStateMachineTest, drive_to_init_state_on_right_inverter_fault)
{
    auto set_fault   = []() { app_canRx_INVR_VsmState_update(INVERTER_VSM_BLINK_FAULT_CODE_STATE); };
    auto clear_fault = []() { app_canRx_INVR_VsmState_update(INVERTER_VSM_START_STATE); };

    TestFaultBlocksDrive(set_fault, clear_fault);
}

TEST_F(VCStateMachineTest, drive_to_init_state_on_bms_fault)
{
    auto set_fault   = []() { app_canRx_BMS_Fault_CellOvervoltage_update(true); };
    auto clear_fault = []() { app_canRx_BMS_Fault_CellOvervoltage_update(false); };

    TestFaultBlocksDrive(set_fault, clear_fault);
}

// TEST_F(VCStateMachineTest, drive_to_init_state_on_fsm_fault)
// {
//     auto set_fault   = []() { app_canRx_FSM_Fault_FlowMeterUnderflow_update(true); };
//     auto clear_fault = []() { app_canRx_FSM_Fault_FlowMeterUnderflow_update(false); };

//     TestFaultBlocksDrive(set_fault, clear_fault);
// }

// TEST_F(VCStateMachineTest, drive_to_init_state_on_VC_fault)
// {
//     auto set_fault   = []() { app_canRx_VC_Fault_DummyFault_update(true); };
//     auto clear_fault = []() { app_canRx_VC_Fault_DummyFault_update(false); };

//     TestFaultBlocksDrive(set_fault, clear_fault);
// }

TEST_F(VCStateMachineTest, drive_to_init_state_on_CRIT_fault)
{
    auto set_fault   = []() { app_canRx_CRIT_Fault_MissingBMSHeartbeat_update(true); };
    auto clear_fault = []() { app_canRx_CRIT_Fault_MissingBMSHeartbeat_update(false); };

    TestFaultBlocksDrive(set_fault, clear_fault);
}
