#include "fff.h"
#include "gtest/gtest.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "states/App_InitState.h"
#include "states/App_RunState.h"

    DEFINE_FFF_GLOBALS;
    FAKE_VOID_FUNC(
        send_non_periodic_msg_DCM_STARTUP,
        struct CanMsgs_dcm_startup_t *);
    FAKE_VOID_FUNC(
        send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT,
        struct CanMsgs_dcm_watchdog_timeout_t *);
}

class DcmStateMachineTest : public testing::Test
{
  protected:
    virtual void SetUp()
    {
        can_tx_interface = App_CanTx_Create(
            send_non_periodic_msg_DCM_STARTUP,
            send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT);
        can_rx_interface = App_CanRx_Create();
        world = App_DcmWorld_Create(can_tx_interface, can_rx_interface);

        // Default to starting the state machine in the `init` state
        state_machine =
            App_SharedStateMachine_Create(world, App_GetInitState());

        // Reset fake functions
        RESET_FAKE(send_non_periodic_msg_DCM_STARTUP);
        RESET_FAKE(send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT);
    }

    virtual void SetInitialState(const struct State *initial_state)
    {
        assert(initial_state != NULL);
        if (state_machine != NULL)
        {
            App_SharedStateMachine_Destroy(state_machine);
        }
        state_machine = App_SharedStateMachine_Create(world, initial_state);
        EXPECT_TRUE(state_machine);
    }

    virtual void TearDown()
    {
        if (state_machine != NULL)
        {
            App_SharedStateMachine_Destroy(state_machine);
        }
        if (world != NULL)
        {
            App_DcmWorld_Destroy(world);
        }
        world         = NULL;
        state_machine = NULL;
    }

    struct StateMachine *     state_machine;
    struct World *            world;
    struct DCMCanTxInterface *can_tx_interface;
    struct DCMCanRxInterface *can_rx_interface;
};

TEST_F(
    DcmStateMachineTest,
    check_startup_message_is_broadcasted_on_init_state_entry)
{
    SetInitialState(App_GetInitState());

    ASSERT_EQ(1, send_non_periodic_msg_DCM_STARTUP_fake.call_count);
}

TEST_F(
    DcmStateMachineTest,
    check_init_immediately_transitions_to_run_on_first_tick)
{
    SetInitialState(App_GetInitState());

    // We need to tick twice, once to run the `Init` state, and once more
    // to have the state machine transition to the `Run` state.
    App_SharedStateMachine_Tick(state_machine);
    App_SharedStateMachine_Tick(state_machine);

    EXPECT_EQ(
        App_GetRunState(),
        App_SharedStateMachine_GetCurrentState(state_machine));
}
