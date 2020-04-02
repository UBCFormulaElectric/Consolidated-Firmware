#include "fff.h"
#include "gtest/gtest.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "states/App_InitState.h"

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
    }
    virtual void TearDown() {}

    struct World *            world;
    struct DCMCanTxInterface *can_tx_interface;
    struct DCMCanRxInterface *can_rx_interface;
};

TEST_F(
    DcmStateMachineTest,
    check_startup_message_is_broadcasted_on_init_state_entry)
{
    struct StateMachine *state_machine =
        App_SharedStateMachine_Create(world, getInitState());

    ASSERT_NE((size_t)state_machine, NULL);

    ASSERT_EQ(1, send_non_periodic_msg_DCM_STARTUP_fake.call_count);
}

TEST_F(
    DcmStateMachineTest,
    check_init_immediately_transitions_to_run_on_first_tick)
{
    struct StateMachine *state_machine =
        App_SharedStateMachine_Create(world, getInitState());

    ASSERT_NE((size_t)state_machine, NULL);

    // We need to tick twice, once to run the `Init` state, and once more
    // to have the state machine transition to the `Run` state.
    App_SharedStateMachine_Tick(state_machine);
    App_SharedStateMachine_Tick(state_machine);

    EXPECT_EQ(
        getRunState(), App_SharedStateMachine_GetCurrentState(state_machine));
}
