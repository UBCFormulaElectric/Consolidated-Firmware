#include "test_dcmBaseStateMachineTest.h"

class DcmStateMachineTest : public DcmBaseStateMachineTest
{
  protected:
    void TestFaultBlocksDrive(std::function<void(void)> set_fault, std::function<void(void)> clear_fault)
    {
        SetInitialState(app_driveState_get());

        // Set the DIM start switch to on, and the BMS to drive state, to prevent state transitions in
        // the drive state.
        App_CanRx_DIM_StartSwitch_Update(SWITCH_ON);
        App_CanRx_BMS_State_Update(BMS_DRIVE_STATE);
        LetTimePass(state_machine, 10);
        EXPECT_EQ(DCM_DRIVE_STATE, App_CanTx_DCM_State_Get());

        // Introduce fault, expect transition to init state.
        set_fault();
        LetTimePass(state_machine, 10);
        EXPECT_EQ(DCM_INIT_STATE, App_CanTx_DCM_State_Get());

        // Confirm we don't allow a transition back to drive until the fault clears.
        LetTimePass(state_machine, 1000);
        EXPECT_EQ(DCM_INIT_STATE, App_CanTx_DCM_State_Get());

        // Clear fault and observe a transition back to drive, when the drive conditions are met.
        clear_fault();

        App_CanRx_FSM_BrakeActuated_Update(true);
        App_CanRx_DIM_StartSwitch_Update(SWITCH_OFF);
        LetTimePass(state_machine, 10);
        App_CanRx_DIM_StartSwitch_Update(SWITCH_ON);
        LetTimePass(state_machine, 10);
        EXPECT_EQ(DCM_DRIVE_STATE, App_CanTx_DCM_State_Get());
    }
};

TEST_F(DcmStateMachineTest, check_init_transitions_to_drive_if_conditions_met_and_start_switch_pulled_up)
{
    // Pull start switch down and back up, expect no transition
    App_CanRx_DIM_StartSwitch_Update(SWITCH_OFF);
    App_CanRx_DIM_StartSwitch_Update(SWITCH_ON);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(app_initState_get(), App_SharedStateMachine_GetCurrentState(state_machine));

    // Transition BMS to drive state, expect no transition
    App_CanRx_BMS_State_Update(BMS_DRIVE_STATE);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(app_initState_get(), App_SharedStateMachine_GetCurrentState(state_machine));

    // Actuate brake pedal
    App_CanRx_FSM_BrakeActuated_Update(true);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(app_initState_get(), App_SharedStateMachine_GetCurrentState(state_machine));

    // Pull start switch down and back up, expect init->drive transition
    App_CanRx_DIM_StartSwitch_Update(SWITCH_OFF);
    LetTimePass(state_machine, 10);
    App_CanRx_DIM_StartSwitch_Update(SWITCH_ON);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(DCM_DRIVE_STATE, App_CanTx_DCM_State_Get());
}

TEST_F(DcmStateMachineTest, check_init_state_is_broadcasted_over_can)
{
    SetInitialState(app_initState_get());
    EXPECT_EQ(DCM_INIT_STATE, App_CanTx_DCM_State_Get());
}

TEST_F(DcmStateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(app_driveState_get());

    EXPECT_EQ(DCM_DRIVE_STATE, App_CanTx_DCM_State_Get());
}

TEST_F(DcmStateMachineTest, disable_inverters_in_init_state)
{
    // Start in drive with a non-zero torque request to prevent false positive.
    SetInitialState(app_driveState_get());
    App_CanTx_DCM_LeftInverterTorqueCommand_Set(1.0f);
    App_CanTx_DCM_RightInverterTorqueCommand_Set(1.0f);
    App_CanTx_DCM_LeftInverterEnable_Set(true);
    App_CanTx_DCM_RightInverterEnable_Set(true);

    EXPECT_FLOAT_EQ(1.0f, App_CanTx_DCM_LeftInverterTorqueCommand_Get());
    EXPECT_FLOAT_EQ(1.0f, App_CanTx_DCM_RightInverterTorqueCommand_Get());
    EXPECT_FLOAT_EQ(true, App_CanTx_DCM_LeftInverterEnable_Get());
    EXPECT_FLOAT_EQ(true, App_CanTx_DCM_RightInverterEnable_Get());

    // Flip start switch down causing transition to init state.
    App_CanRx_DIM_StartSwitch_Update(SWITCH_OFF);

    // Now tick the state machine and check torque request gets zeroed on transition to init.
    LetTimePass(state_machine, 10);
    EXPECT_FLOAT_EQ(0.0f, App_CanTx_DCM_LeftInverterTorqueCommand_Get());
    EXPECT_FLOAT_EQ(0.0f, App_CanTx_DCM_RightInverterTorqueCommand_Get());
    EXPECT_FLOAT_EQ(false, App_CanTx_DCM_LeftInverterEnable_Get());
    EXPECT_FLOAT_EQ(false, App_CanTx_DCM_RightInverterEnable_Get());
}

TEST_F(DcmStateMachineTest, start_switch_off_transitions_drive_state_to_init_state)
{
    SetInitialState(app_driveState_get());
    App_CanRx_DIM_StartSwitch_Update(SWITCH_OFF);
    LetTimePass(state_machine, 10);

    ASSERT_EQ(DCM_INIT_STATE, App_CanTx_DCM_State_Get());
}

TEST_F(DcmStateMachineTest, check_if_buzzer_stays_on_for_two_seconds_only_after_entering_drive_state)
{
    for (auto &state : GetAllStates())
    {
        BaseStateMachineTest::SetUp();
        SetInitialState(state);

        if (App_CanTx_DCM_State_Get() == DCM_DRIVE_STATE)
        {
            // Set the DIM start switch to on, and the BMS to drive state, to prevent state transitions in
            // the drive state.
            App_CanRx_DIM_StartSwitch_Update(SWITCH_ON);
            App_CanRx_BMS_State_Update(BMS_DRIVE_STATE);
            EXPECT_TRUE(App_CanTx_DCM_BuzzerOn_Get());

            LetTimePass(state_machine, BUZZER_ON_DURATION_MS);
            EXPECT_TRUE(App_CanTx_DCM_BuzzerOn_Get());

            // Check that the buzzer has been turned off after waiting for
            // BUZZER_ON_DURATION_MS.
            LetTimePass(state_machine, 10);
            EXPECT_FALSE(App_CanTx_DCM_BuzzerOn_Get());

            // Check that the buzzer stays off indefinitely.
            LetTimePass(state_machine, 100);
            EXPECT_FALSE(App_CanTx_DCM_BuzzerOn_Get());
        }
        else
        {
            // Check that the buzzer is not turned on in other states.
            EXPECT_FALSE(App_CanTx_DCM_BuzzerOn_Get());
        }
    }
}

TEST_F(DcmStateMachineTest, no_torque_requests_when_accelerator_pedal_is_not_pressed)
{
    SetInitialState(app_driveState_get());

    // Set the DIM start switch to on, and the BMS to drive state, to prevent state transitions in
    // the drive state.
    App_CanRx_DIM_StartSwitch_Update(SWITCH_ON);
    App_CanRx_BMS_State_Update(BMS_DRIVE_STATE);

    // Check that no torque requests are sent when the accelerator pedal is not
    // pressed
    LetTimePass(state_machine, 10);
    EXPECT_FLOAT_EQ(0.0f, App_CanTx_DCM_LeftInverterTorqueCommand_Get());
    EXPECT_FLOAT_EQ(0.0f, App_CanTx_DCM_RightInverterTorqueCommand_Get());
}

TEST_F(DcmStateMachineTest, drive_to_init_state_on_left_inverter_fault)
{
    auto set_fault   = []() { App_CanRx_INVR_VsmState_Update(INVERTER_VSM_BLINK_FAULT_CODE_STATE); };
    auto clear_fault = []() { App_CanRx_INVR_VsmState_Update(INVERTER_VSM_START_STATE); };

    TestFaultBlocksDrive(set_fault, clear_fault);
}

TEST_F(DcmStateMachineTest, drive_to_init_state_on_right_inverter_fault)
{
    auto set_fault   = []() { App_CanRx_INVR_VsmState_Update(INVERTER_VSM_BLINK_FAULT_CODE_STATE); };
    auto clear_fault = []() { App_CanRx_INVR_VsmState_Update(INVERTER_VSM_START_STATE); };

    TestFaultBlocksDrive(set_fault, clear_fault);
}

TEST_F(DcmStateMachineTest, drive_to_init_state_on_bms_fault)
{
    auto set_fault   = []() { App_CanRx_BMS_Fault_CellOvervoltage_Update(true); };
    auto clear_fault = []() { App_CanRx_BMS_Fault_CellOvervoltage_Update(false); };

    TestFaultBlocksDrive(set_fault, clear_fault);
}

TEST_F(DcmStateMachineTest, drive_to_init_state_on_fsm_fault)
{
    auto set_fault   = []() { App_CanRx_FSM_Fault_FlowMeterUnderflow_Update(true); };
    auto clear_fault = []() { App_CanRx_FSM_Fault_FlowMeterUnderflow_Update(false); };

    TestFaultBlocksDrive(set_fault, clear_fault);
}

TEST_F(DcmStateMachineTest, drive_to_init_state_on_pdm_fault)
{
    auto set_fault   = []() { App_CanRx_PDM_Fault_DummyFault_Update(true); };
    auto clear_fault = []() { App_CanRx_PDM_Fault_DummyFault_Update(false); };

    TestFaultBlocksDrive(set_fault, clear_fault);
}

TEST_F(DcmStateMachineTest, drive_to_init_state_on_dim_fault)
{
    auto set_fault   = []() { App_CanRx_DIM_Fault_MissingBMSHeartbeat_Update(true); };
    auto clear_fault = []() { App_CanRx_DIM_Fault_MissingBMSHeartbeat_Update(false); };

    TestFaultBlocksDrive(set_fault, clear_fault);
}
