#include <math.h>
#include "Test_Dcm.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "states/App_InitState.h"
#include "states/App_DriveState.h"
#include "states/App_FaultState.h"
#include "configs/App_HeartbeatMonitorConfig.h"
}

namespace StateMachineTest
{
FAKE_VOID_FUNC(
    send_non_periodic_msg_DCM_STARTUP,
    const struct CanMsgs_dcm_startup_t *);
FAKE_VOID_FUNC(
    send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT,
    const struct CanMsgs_dcm_watchdog_timeout_t *);
FAKE_VALUE_FUNC(uint32_t, get_current_ms);
FAKE_VOID_FUNC(
    heartbeat_timeout_callback,
    enum HeartbeatOneHot,
    enum HeartbeatOneHot);
FAKE_VOID_FUNC(turn_on_red_led);
FAKE_VOID_FUNC(turn_on_green_led);
FAKE_VOID_FUNC(turn_on_blue_led);
FAKE_VOID_FUNC(turn_on_brake_light);
FAKE_VOID_FUNC(turn_off_brake_light);

class DcmStateMachineTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        can_tx_interface = App_CanTx_Create(
            send_non_periodic_msg_DCM_STARTUP,
            send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT);

        can_rx_interface = App_CanRx_Create();

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS,
            HEARTBEAT_MONITOR_BOARDS_TO_CHECK, heartbeat_timeout_callback);

        rgb_led_sequence = App_SharedRgbLedSequence_Create(
            turn_on_red_led, turn_on_green_led, turn_on_blue_led);

        brake_light =
            App_BrakeLight_Create(turn_on_brake_light, turn_off_brake_light);

        world = App_DcmWorld_Create(
            can_tx_interface, can_rx_interface, heartbeat_monitor,
            rgb_led_sequence, brake_light);

        // Default to starting the state machine in the `init` state
        state_machine =
            App_SharedStateMachine_Create(world, App_GetInitState());

        RESET_FAKE(send_non_periodic_msg_DCM_STARTUP);
        RESET_FAKE(send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT);
        RESET_FAKE(get_current_ms);
        RESET_FAKE(heartbeat_timeout_callback);
        RESET_FAKE(turn_on_red_led);
        RESET_FAKE(turn_on_green_led);
        RESET_FAKE(turn_on_blue_led);
        RESET_FAKE(turn_on_brake_light);
        RESET_FAKE(turn_off_brake_light);
    }

    void TearDown() override
    {
        TearDownObject(world, App_DcmWorld_Destroy);
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        TearDownObject(can_tx_interface, App_CanTx_Destroy);
        TearDownObject(can_rx_interface, App_CanRx_Destroy);
        TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy);
        TearDownObject(rgb_led_sequence, App_SharedRgbLedSequence_Destroy);
    }

    void SetInitialState(const struct State *const initial_state)
    {
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        state_machine = App_SharedStateMachine_Create(world, initial_state);
        ASSERT_EQ(
            initial_state,
            App_SharedStateMachine_GetCurrentState(state_machine));
    }

    std::vector<const struct State *> GetAllStates(void)
    {
        return std::vector<const struct State *>{
            App_GetInitState(),
            App_GetDriveState(),
            App_GetFaultState(),
        };
    }

    struct World *            world;
    struct StateMachine *     state_machine;
    struct DcmCanTxInterface *can_tx_interface;
    struct DcmCanRxInterface *can_rx_interface;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct RgbLedSequence *   rgb_led_sequence;
    struct BrakeLight *       brake_light;
};

TEST_F(
    DcmStateMachineTest,
    check_init_immediately_transitions_to_run_on_first_tick)
{
    // We need to tick twice, once to run the `Init` state, and once more
    // to have the state machine transition to the `Run` state.
    App_SharedStateMachine_Tick(state_machine);
    App_SharedStateMachine_Tick(state_machine);

    EXPECT_EQ(
        App_GetDriveState(),
        App_SharedStateMachine_GetCurrentState(state_machine));
}

TEST_F(DcmStateMachineTest, check_init_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetInitState());

    EXPECT_EQ(
        CANMSGS_DCM_STATE_MACHINE_STATE_INIT_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

TEST_F(DcmStateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetDriveState());

    EXPECT_EQ(
        CANMSGS_DCM_STATE_MACHINE_STATE_DRIVE_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

TEST_F(DcmStateMachineTest, check_fault_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetFaultState());

    EXPECT_EQ(
        CANMSGS_DCM_STATE_MACHINE_STATE_FAULT_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

TEST_F(DcmStateMachineTest, brake_light_control_in_all_states)
{
    for (const auto &state : GetAllStates())
    {
        SetInitialState(state);

        // Brake = Not actuated
        // Regen = Not active
        App_CanRx_FSM_BRAKE_SetSignal_BRAKE_IS_ACTUATED(
            can_rx_interface, false);
        App_CanTx_SetPeriodicSignal_TORQUE_REQUEST(can_tx_interface, 0.0f);
        App_SharedStateMachine_Tick(state_machine);
        ASSERT_EQ(App_BrakeLight_IsTurnedOn(brake_light), false);
        ASSERT_EQ(turn_on_brake_light_fake.call_count, 0);
        ASSERT_EQ(turn_off_brake_light_fake.call_count, 1);

        // Brake = Not actuated
        // Regen = Active
        App_CanRx_FSM_BRAKE_SetSignal_BRAKE_IS_ACTUATED(
            can_rx_interface, false);
        App_CanTx_SetPeriodicSignal_TORQUE_REQUEST(
            can_tx_interface,
            std::nextafter(0.0f, std::numeric_limits<float>::lowest()));
        App_SharedStateMachine_Tick(state_machine);
        ASSERT_EQ(App_BrakeLight_IsTurnedOn(brake_light), true);
        ASSERT_EQ(turn_on_brake_light_fake.call_count, 1);
        ASSERT_EQ(turn_off_brake_light_fake.call_count, 1);

        // Brake = Actuated
        // Regen = Not active
        App_CanRx_FSM_BRAKE_SetSignal_BRAKE_IS_ACTUATED(can_rx_interface, true);
        App_CanTx_SetPeriodicSignal_TORQUE_REQUEST(can_tx_interface, 0.0f);
        App_SharedStateMachine_Tick(state_machine);
        ASSERT_EQ(App_BrakeLight_IsTurnedOn(brake_light), true);
        ASSERT_EQ(turn_on_brake_light_fake.call_count, 2);
        ASSERT_EQ(turn_off_brake_light_fake.call_count, 1);

        // Brake = Actuated
        // Regen = Active
        App_CanRx_FSM_BRAKE_SetSignal_BRAKE_IS_ACTUATED(can_rx_interface, true);
        App_CanTx_SetPeriodicSignal_TORQUE_REQUEST(
            can_tx_interface,
            std::nextafter(0.0f, std::numeric_limits<float>::lowest()));
        App_SharedStateMachine_Tick(state_machine);
        ASSERT_EQ(App_BrakeLight_IsTurnedOn(brake_light), true);
        ASSERT_EQ(turn_on_brake_light_fake.call_count, 3);
        ASSERT_EQ(turn_off_brake_light_fake.call_count, 1);

        // Manually reset the call count for the fake functions
        RESET_FAKE(turn_on_brake_light);
        RESET_FAKE(turn_off_brake_light);
    }
}

} // namespace StateMachineTest
