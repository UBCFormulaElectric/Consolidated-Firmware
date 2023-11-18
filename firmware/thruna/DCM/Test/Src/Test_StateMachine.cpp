#include <math.h>
#include <functional>
#include "Test_Dcm.h"
#include "Test_BaseStateMachineTest.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "App_SharedMacros.h"
#include "App_SharedConstants.h"
#include "states/App_InitState.h"
#include "states/App_DriveState.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "configs/App_WaitSignalDuration.h"
#include "configs/App_AccelerationThresholds.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanUtils.h"
}

namespace StateMachineTest
{
FAKE_VALUE_FUNC(uint32_t, get_current_ms);
FAKE_VOID_FUNC(heartbeat_timeout_callback, enum HeartbeatOneHot, enum HeartbeatOneHot);
FAKE_VOID_FUNC(turn_on_red_led);
FAKE_VOID_FUNC(turn_on_green_led);
FAKE_VOID_FUNC(turn_on_blue_led);
FAKE_VOID_FUNC(turn_on_brake_light);
FAKE_VOID_FUNC(turn_off_brake_light);
FAKE_VOID_FUNC(turn_on_buzzer);
FAKE_VOID_FUNC(turn_off_buzzer);
FAKE_VALUE_FUNC(float, get_acceleration_x);
FAKE_VALUE_FUNC(float, get_acceleration_y);
FAKE_VALUE_FUNC(float, get_acceleration_z);
FAKE_VOID_FUNC(turn_on_right_inverter);
FAKE_VOID_FUNC(turn_off_right_inverter);
FAKE_VOID_FUNC(turn_on_left_inverter);
FAKE_VOID_FUNC(turn_off_left_inverter);
FAKE_VALUE_FUNC(bool, is_right_inverter_on);
FAKE_VALUE_FUNC(bool, is_left_inverter_on);
FAKE_VOID_FUNC(handle_imu_logs);
FAKE_VALUE_FUNC(uint32_t, get_imu_timestamp);
FAKE_VALUE_FUNC(uint16_t, get_imu_general_status);
FAKE_VALUE_FUNC(uint32_t, get_imu_com_status);
FAKE_VALUE_FUNC(float, get_imu_output, EllipseImuOutput);

class DcmStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        App_CanTx_Init();

        App_CanRx_Init();

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK);

        brake_light = App_BrakeLight_Create(turn_on_brake_light, turn_off_brake_light);

        buzzer = App_Buzzer_Create(turn_on_buzzer, turn_off_buzzer);

        imu = App_Imu_Create(
            get_acceleration_x, get_acceleration_y, get_acceleration_z, MIN_ACCELERATION_MS2, MAX_ACCELERATION_MS2);

        App_EllipseImu_Init(
            handle_imu_logs, get_imu_timestamp, get_imu_general_status, get_imu_com_status, get_imu_output);

        clock = App_SharedClock_Create();

        world = App_DcmWorld_Create(
            heartbeat_monitor, brake_light, buzzer, imu, clock, App_BuzzerSignals_IsOn, App_BuzzerSignals_Callback);

        // Default to starting the state machine in the `init` state
        state_machine = App_SharedStateMachine_Create(world, App_GetInitState());

        RESET_FAKE(get_current_ms);
        RESET_FAKE(get_acceleration_x);
        RESET_FAKE(get_acceleration_y);
        RESET_FAKE(get_acceleration_z);
        RESET_FAKE(heartbeat_timeout_callback);
        RESET_FAKE(turn_on_red_led);
        RESET_FAKE(turn_on_green_led);
        RESET_FAKE(turn_on_blue_led);
        RESET_FAKE(turn_on_brake_light);
        RESET_FAKE(turn_off_brake_light);
        RESET_FAKE(turn_on_right_inverter);
        RESET_FAKE(turn_off_right_inverter);
        RESET_FAKE(turn_on_left_inverter);
        RESET_FAKE(turn_off_left_inverter);
    }

    void TearDown() override
    {
        TearDownObject(world, App_DcmWorld_Destroy);
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy);
        TearDownObject(brake_light, App_BrakeLight_Destroy);
        TearDownObject(buzzer, App_Buzzer_Destroy);
        TearDownObject(imu, App_Imu_Destroy);
        TearDownObject(clock, App_SharedClock_Destroy);
    }

    void SetInitialState(const struct State *const initial_state)
    {
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        state_machine = App_SharedStateMachine_Create(world, initial_state);
        ASSERT_EQ(initial_state, App_SharedStateMachine_GetCurrentState(state_machine));
    }

    std::vector<const struct State *> GetAllStates(void)
    {
        return std::vector<const struct State *>{
            App_GetInitState(),
            App_GetDriveState(),
        };
    }

    void UpdateClock(struct StateMachine *state_machine, uint32_t current_time_ms) override
    {
        struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
        struct Clock *   clock = App_DcmWorld_GetClock(world);
        App_SharedClock_SetCurrentTimeInMilliseconds(clock, current_time_ms);
    }

    void UpdateSignals(struct StateMachine *state_machine, uint32_t current_time_ms) override
    {
        struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
        App_DcmWorld_UpdateWaitSignal(world, current_time_ms);
    }

    void TestFaultBlocksDrive(std::function<void(void)> set_fault, std::function<void(void)> clear_fault)
    {
        SetInitialState(App_GetDriveState());

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

    struct World *            world;
    struct StateMachine *     state_machine;
    struct DcmCanTxInterface *can_tx_interface;
    struct DcmCanRxInterface *can_rx_interface;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct BrakeLight *       brake_light;
    struct Buzzer *           buzzer;
    struct Imu *              imu;
    struct Clock *            clock;
};

TEST_F(DcmStateMachineTest, check_init_transitions_to_drive_if_conditions_met_and_start_switch_pulled_up)
{
    // Pull start switch down and back up, expect no transition
    App_CanRx_DIM_StartSwitch_Update(SWITCH_OFF);
    App_CanRx_DIM_StartSwitch_Update(SWITCH_ON);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));

    // Transition BMS to drive state, expect no transition
    App_CanRx_BMS_State_Update(BMS_DRIVE_STATE);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));

    // Actuate brake pedal
    App_CanRx_FSM_BrakeActuated_Update(true);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));

    // Pull start switch down and back up, expect init->drive transition
    App_CanRx_DIM_StartSwitch_Update(SWITCH_OFF);
    LetTimePass(state_machine, 10);
    App_CanRx_DIM_StartSwitch_Update(SWITCH_ON);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(DCM_DRIVE_STATE, App_CanTx_DCM_State_Get());
}

TEST_F(DcmStateMachineTest, check_init_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetInitState());
    EXPECT_EQ(DCM_INIT_STATE, App_CanTx_DCM_State_Get());
}

TEST_F(DcmStateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetDriveState());

    EXPECT_EQ(DCM_DRIVE_STATE, App_CanTx_DCM_State_Get());
}

TEST_F(DcmStateMachineTest, disable_inverters_in_init_state)
{
    // Start in drive with a non-zero torque request to prevent false positive.
    SetInitialState(App_GetDriveState());
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
    SetInitialState(App_GetDriveState());
    App_CanRx_DIM_StartSwitch_Update(SWITCH_OFF);
    LetTimePass(state_machine, 10);

    ASSERT_EQ(DCM_INIT_STATE, App_CanTx_DCM_State_Get());
}

TEST_F(DcmStateMachineTest, check_if_buzzer_stays_on_for_two_seconds_only_after_entering_drive_state)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        if (DCM_DRIVE_STATE == App_CanTx_DCM_State_Get())
        {
            // Set the DIM start switch to on, and the BMS to drive state, to prevent state transitions in
            // the drive state.
            App_CanRx_DIM_StartSwitch_Update(SWITCH_ON);
            App_CanRx_BMS_State_Update(BMS_DRIVE_STATE);

            EXPECT_TRUE(App_BuzzerSignals_IsOn(world));

            LetTimePass(state_machine, BUZZER_ON_DURATION_MS - 1);
            EXPECT_TRUE(App_BuzzerSignals_IsOn(world));

            // Check that the buzzer has been turned off after waiting for
            // BUZZER_ON_DURATION_MS.
            LetTimePass(state_machine, 1);
            EXPECT_FALSE(App_BuzzerSignals_IsOn(world));

            // Check that the buzzer stays off for the next 100ms. It should
            // be enough time to give us confidence in the test without making
            // the test too slow.
            LetTimePass(state_machine, 100);
            EXPECT_FALSE(App_BuzzerSignals_IsOn(world));
        }
        else
        {
            // Check that the buzzer is not turned on in other states.
            EXPECT_FALSE(App_BuzzerSignals_IsOn(world));
        }
    }
}

TEST_F(DcmStateMachineTest, no_torque_requests_when_accelerator_pedal_is_not_pressed)
{
    SetInitialState(App_GetDriveState());

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
    auto set_fault   = []() { App_CanRx_DIM_Fault_MissingHeartbeat_Update(true); };
    auto clear_fault = []() { App_CanRx_DIM_Fault_MissingHeartbeat_Update(false); };

    TestFaultBlocksDrive(set_fault, clear_fault);
}

} // namespace StateMachineTest
