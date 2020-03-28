#include "gtest/gtest.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "states/App_InitState.h"
}

class DcmStateMachineTest : public testing::Test
{
  protected:
    virtual void SetUp()
    {
        send_non_periodic_msg_DCM_STARTUP_was_called          = false;
        send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT_was_called = false;

        struct CanTxInterface *can_tx_interface = App_CanTx_Create(
            send_non_periodic_msg_DCM_STARTUP,
            send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT);
        world = App_DcmWorld_Create(can_tx_interface);
    }
    virtual void TearDown() {}

    static void
        send_non_periodic_msg_DCM_STARTUP(struct CanMsgs_dcm_startup_t *payload)
    {
        send_non_periodic_msg_DCM_STARTUP_was_called = true;
    }

    static void send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT(
        struct CanMsgs_dcm_watchdog_timeout_t *payload)
    {
        send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT_was_called = true;
    }

    inline static bool send_non_periodic_msg_DCM_STARTUP_was_called;
    inline static bool send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT_was_called;

    struct World *world;
};

TEST_F(
    DcmStateMachineTest,
    check_startup_message_is_broadcasted_on_init_state_entry)
{
    struct StateMachine *state_machine =
        App_SharedStateMachine_Create(world, getInitState());

    ASSERT_TRUE(send_non_periodic_msg_DCM_STARTUP_was_called);
}
