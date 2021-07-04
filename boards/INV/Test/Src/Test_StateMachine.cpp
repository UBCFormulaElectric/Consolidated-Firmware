#include "Test_Inv.h"
#include "Test_BaseStateMachineTest.h"

extern "C"
{
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedStateMachine.h"
#include "App_SharedRgbLedSequence.h"
#include "App_SharedMacros.h"
#include "states/App_DriveState.h"
#include "configs/App_HeartbeatMonitorConfig.h"
}

namespace StateMachineTest
{
FAKE_VOID_FUNC(
    send_non_periodic_msg_INV_STARTUP,
    const struct CanMsgs_inv_startup_t *);
FAKE_VOID_FUNC(
    send_non_periodic_msg_INV_WATCHDOG_TIMEOUT,
    const struct CanMsgs_inv_watchdog_timeout_t *);
FAKE_VOID_FUNC(turn_on_red_led);
FAKE_VOID_FUNC(turn_on_green_led);
FAKE_VOID_FUNC(turn_on_blue_led);

FAKE_VALUE_FUNC(uint32_t, get_current_ms);
FAKE_VOID_FUNC(
    heartbeat_timeout_callback,
    enum HeartbeatOneHot,
    enum HeartbeatOneHot);

class InvStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        can_tx_interface = App_CanTx_Create(
            send_non_periodic_msg_INV_STARTUP,
            send_non_periodic_msg_INV_WATCHDOG_TIMEOUT);
        can_rx_interface = App_CanRx_Create();

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS,
            HEARTBEAT_MONITOR_BOARDS_TO_CHECK, heartbeat_timeout_callback);

        rgb_led_sequence = App_SharedRgbLedSequence_Create(
            turn_on_red_led, turn_on_green_led, turn_on_blue_led);

        error_table = App_SharedErrorTable_Create();

        clock = App_SharedClock_Create();

        world = App_InvWorld_Create(
            can_tx_interface, can_rx_interface, heartbeat_monitor,
            rgb_led_sequence, error_table, clock);

        // Default to starting the state machine in the `Drive` state
        state_machine =
            App_SharedStateMachine_Create(world, App_GetDriveState());

        // Reset fake functions
        RESET_FAKE(send_non_periodic_msg_INV_STARTUP);
        RESET_FAKE(send_non_periodic_msg_INV_WATCHDOG_TIMEOUT);
        RESET_FAKE(get_current_ms);
        RESET_FAKE(heartbeat_timeout_callback);
    }

    void TearDown() override
    {
        TearDownObject(world, App_InvWorld_Destroy);
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        TearDownObject(can_tx_interface, App_CanTx_Destroy);
        TearDownObject(can_rx_interface, App_CanRx_Destroy);
        TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy);
        TearDownObject(rgb_led_sequence, App_SharedRgbLedSequence_Destroy);
        TearDownObject(error_table, App_SharedErrorTable_Destroy);
        TearDownObject(clock, App_SharedClock_Destroy);
    }

    void SetInitialState(const struct State *const initial_state)
    {
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        state_machine = App_SharedStateMachine_Create(world, initial_state);
        ASSERT_EQ(
            initial_state,
            App_SharedStateMachine_GetCurrentState(state_machine));
    }

    void UpdateClock(
        struct StateMachine *state_machine,
        uint32_t             current_time_ms) override
    {
        struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
        struct Clock *   clock = App_InvWorld_GetClock(world);
        App_SharedClock_SetCurrentTimeInMilliseconds(clock, current_time_ms);
    }

    void UpdateSignals(
        struct StateMachine *state_machine,
        uint32_t             current_time_ms) override
    {
        // INV doesn't use any signals currently
        UNUSED(state_machine);
        UNUSED(current_time_ms);
    }

    struct World *            world;
    struct StateMachine *     state_machine;
    struct InvCanTxInterface *can_tx_interface;
    struct InvCanRxInterface *can_rx_interface;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct RgbLedSequence *   rgb_led_sequence;
    struct ErrorTable *       error_table;
    struct Clock *            clock;
};

// TODO: if we want to do things, properly, each of these test should be
//       associated with a requirement in `requirements.md`. See other tests
//       for examples of how we denote this.

// TODO: requirement here
// INV-??
TEST_F(InvStateMachineTest, rgb_led_sequence_in_drive_state)
{
    unsigned int *call_counts[] = { &turn_on_red_led_fake.call_count,
                                    &turn_on_green_led_fake.call_count,
                                    &turn_on_blue_led_fake.call_count };

    // Verify that we cycle through red, green, blue, red, etc. for 99 times.
    // The number 99 can be changed to any other number that is a multiple
    // of 3. The significance of 3 is because we have 3 colors (Red, green,
    // and blue).
    for (size_t i = 0; i < 99; i++)
    {
        LetTimePass(state_machine, 1000);
        ASSERT_EQ(*call_counts[i % 3], i / 3 + 1);
    }
}

} // namespace StateMachineTest
