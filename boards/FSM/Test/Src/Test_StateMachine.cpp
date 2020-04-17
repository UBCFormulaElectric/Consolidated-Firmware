#include "Test_Fsm.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "states/App_AirOpenState.h"
#include "states/App_AirClosedState.h"
}

FAKE_VOID_FUNC(
    send_non_periodic_msg_FSM_STARTUP,
    struct CanMsgs_fsm_startup_t *);
FAKE_VOID_FUNC(
    send_non_periodic_msg_FSM_AIR_SHUTDOWN,
    struct CanMsgs_fsm_air_shutdown_t *);
FAKE_VOID_FUNC(
    send_non_periodic_msg_FSM_WATCHDOG_TIMEOUT,
    struct CanMsgs_fsm_watchdog_timeout_t *);

class FsmStateMachineTest : public FsmTest
{
  protected:
    void SetUp() override
    {
        can_tx_interface = App_CanTx_Create(
            send_non_periodic_msg_FSM_STARTUP,
            send_non_periodic_msg_FSM_WATCHDOG_TIMEOUT,
            send_non_periodic_msg_FSM_AIR_SHUTDOWN);
        can_rx_interface = App_CanRx_Create();
        world = App_FsmWorld_Create(can_tx_interface, can_rx_interface);

        // Default to starting the state machine in the `AIR_OPEN` state
        state_machine =
            App_SharedStateMachine_Create(world, App_GetAirOpenState());

        // Reset fake functions
        RESET_FAKE(send_non_periodic_msg_FSM_STARTUP);
        RESET_FAKE(send_non_periodic_msg_FSM_AIR_SHUTDOWN);
        RESET_FAKE(send_non_periodic_msg_FSM_WATCHDOG_TIMEOUT);
    }

    void SetInitialState(const struct State *const initial_state)
    {
        App_SharedStateMachine_Destroy(state_machine);
        state_machine = App_SharedStateMachine_Create(world, initial_state);
        ASSERT_TRUE(state_machine != NULL);
        ASSERT_EQ(
            initial_state,
            App_SharedStateMachine_GetCurrentState(state_machine));
    }

    void TearDown() override
    {
        ASSERT_TRUE(world != NULL);
        ASSERT_TRUE(can_tx_interface != NULL);
        ASSERT_TRUE(can_rx_interface != NULL);
        ASSERT_TRUE(state_machine != NULL);

        App_FsmWorld_Destroy(world);
        App_CanTx_Destroy(can_tx_interface);
        App_CanRx_Destroy(can_rx_interface);
        App_SharedStateMachine_Destroy(state_machine);

        world            = NULL;
        can_tx_interface = NULL;
        can_rx_interface = NULL;
        state_machine    = NULL;
    }

    struct World *            world;
    struct FsmCanTxInterface *can_tx_interface;
    struct FsmCanRxInterface *can_rx_interface;
    struct StateMachine *     state_machine;
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
