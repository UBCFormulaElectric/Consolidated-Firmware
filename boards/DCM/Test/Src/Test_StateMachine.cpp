#include <gtest/gtest.h>
#include "Test_Dcm.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "states/App_InitState.h"
#include "states/App_DriveState.h"
#include "states/App_FaultState.h"
}

FAKE_VOID_FUNC(
    send_non_periodic_msg_DCM_STARTUP,
    struct CanMsgs_dcm_startup_t *);
FAKE_VOID_FUNC(
    send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT,
    struct CanMsgs_dcm_watchdog_timeout_t *);

class DcmStateMachineTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        can_tx_interface = App_CanTx_Create(
            send_non_periodic_msg_DCM_STARTUP,
            send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT);
        can_rx_interface = App_CanRx_Create();
        world = App_DcmWorld_Create(can_tx_interface, can_rx_interface);

        // Default to starting the state machine in the `init` state
        state_machine =
            App_SharedStateMachine_Create(world, App_GetInitState());
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

        App_DcmWorld_Destroy(world);
        App_CanTx_Destroy(can_tx_interface);
        App_CanRx_Destroy(can_rx_interface);
        App_SharedStateMachine_Destroy(state_machine);

        world            = NULL;
        can_tx_interface = NULL;
        can_rx_interface = NULL;
        state_machine    = NULL;
    }

    struct World *            world;
    struct DcmCanTxInterface *can_tx_interface;
    struct DcmCanRxInterface *can_rx_interface;
    struct StateMachine *     state_machine;
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
