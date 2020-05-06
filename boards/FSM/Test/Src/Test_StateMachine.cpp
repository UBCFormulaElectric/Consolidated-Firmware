#include "Test_Fsm.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_FlowMeter.h"
#include "states/App_AirOpenState.h"
#include "states/App_AirClosedState.h"
#include "configs/App_HeartbeatMonitorConfig.h"
}

namespace StateMachineTest
{
FAKE_VOID_FUNC(
    send_non_periodic_msg_FSM_STARTUP,
    const struct CanMsgs_fsm_startup_t *);
FAKE_VOID_FUNC(
    send_non_periodic_msg_FSM_AIR_SHUTDOWN,
    const struct CanMsgs_fsm_air_shutdown_t *);
FAKE_VOID_FUNC(
    send_non_periodic_msg_FSM_WATCHDOG_TIMEOUT,
    const struct CanMsgs_fsm_watchdog_timeout_t *);
FAKE_VALUE_FUNC(uint32_t, get_current_ms);
FAKE_VOID_FUNC(
    heartbeat_timeout_callback,
    enum HeartbeatOneHot,
    enum HeartbeatOneHot);
FAKE_VALUE_FUNC(float, get_primary_flow_rate);
FAKE_VALUE_FUNC(float, get_secondary_flow_rate);
FAKE_VOID_FUNC(turn_on_red_led);
FAKE_VOID_FUNC(turn_on_green_led);
FAKE_VOID_FUNC(turn_on_blue_led);

class FsmStateMachineTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        can_tx_interface = App_CanTx_Create(
            send_non_periodic_msg_FSM_STARTUP,
            send_non_periodic_msg_FSM_WATCHDOG_TIMEOUT,
            send_non_periodic_msg_FSM_AIR_SHUTDOWN);

        can_rx_interface = App_CanRx_Create();

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS,
            HEARTBEAT_MONITOR_BOARDS_TO_CHECK, heartbeat_timeout_callback);

        primary_flow_meter = App_FlowMeter_Create(get_primary_flow_rate);

        secondary_flow_meter = App_FlowMeter_Create(get_secondary_flow_rate);

        rgb_led_sequence = App_SharedRgbLedSequence_Create(
            turn_on_red_led, turn_on_green_led, turn_on_blue_led);

        world = App_FsmWorld_Create(
            can_tx_interface, can_rx_interface, heartbeat_monitor,
            primary_flow_meter, secondary_flow_meter, rgb_led_sequence);

        // Default to starting the state machine in the `AIR_OPEN` state
        state_machine =
            App_SharedStateMachine_Create(world, App_GetAirOpenState());

        // Reset fake functions
        RESET_FAKE(send_non_periodic_msg_FSM_STARTUP);
        RESET_FAKE(send_non_periodic_msg_FSM_AIR_SHUTDOWN);
        RESET_FAKE(send_non_periodic_msg_FSM_WATCHDOG_TIMEOUT);
        RESET_FAKE(get_current_ms);
        RESET_FAKE(heartbeat_timeout_callback);
        RESET_FAKE(get_primary_flow_rate);
        RESET_FAKE(get_secondary_flow_rate);
        RESET_FAKE(turn_on_red_led);
        RESET_FAKE(turn_on_green_led);
        RESET_FAKE(turn_on_blue_led);
    }

    void TearDown() override
    {
        TearDownObject(world, App_FsmWorld_Destroy);
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        TearDownObject(can_tx_interface, App_CanTx_Destroy);
        TearDownObject(can_rx_interface, App_CanRx_Destroy);
        TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy);
        TearDownObject(primary_flow_meter, App_FlowMeter_Destroy);
        TearDownObject(secondary_flow_meter, App_FlowMeter_Destroy);
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
        return std::vector<const struct State *>{ App_GetAirOpenState(),
                                                  App_GetAirClosedState() };
    }

    struct World *            world;
    struct StateMachine *     state_machine;
    struct FsmCanTxInterface *can_tx_interface;
    struct FsmCanRxInterface *can_rx_interface;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct FlowMeter *        primary_flow_meter;
    struct FlowMeter *        secondary_flow_meter;
    struct RgbLedSequence *   rgb_led_sequence;
};

TEST_F(FsmStateMachineTest, check_air_open_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetAirOpenState());

    EXPECT_EQ(
        CANMSGS_FSM_STATE_MACHINE_STATE_AIR_OPEN_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

TEST_F(FsmStateMachineTest, check_air_closed_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetAirClosedState());

    EXPECT_EQ(
        CANMSGS_FSM_STATE_MACHINE_STATE_AIR_CLOSED_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

TEST_F(
    FsmStateMachineTest,
    check_if_primary_and_secondary_flow_rates_are_broadcasted_over_can_in_all_states)
{
    float fake_frequency = 1.0f;

    for (const auto &state : GetAllStates())
    {
        SetInitialState(state);
        get_primary_flow_rate_fake.return_val   = fake_frequency;
        get_secondary_flow_rate_fake.return_val = fake_frequency;
        App_SharedStateMachine_Tick(state_machine);

        EXPECT_EQ(
            fake_frequency,
            App_CanTx_GetPeriodicSignal_PRIMARY_FLOW_RATE(can_tx_interface));
        EXPECT_EQ(
            fake_frequency,
            App_CanTx_GetPeriodicSignal_SECONDARY_FLOW_RATE(can_tx_interface));

        // To avoid false positives, we use a different frequency each time
        fake_frequency++;
    }
}

} // namespace StateMachineTest
