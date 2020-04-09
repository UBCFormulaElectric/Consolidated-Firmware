#include "fff.h"
#include "gtest/gtest.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "states/App_InitState.h"
#include "states/App_DriveState.h"
#include "states/App_FaultState.h"
#include "states/App_ChargeState.h"

    DEFINE_FFF_GLOBALS;
    FAKE_VOID_FUNC(
        send_non_periodic_msg_BMS_STARTUP,
        struct CanMsgs_bms_startup_t *);
    FAKE_VOID_FUNC(
        send_non_periodic_msg_BMS_WATCHDOG_TIMEOUT,
        struct CanMsgs_bms_watchdog_timeout_t *);
}

class BmsStateMachineTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        can_tx_interface = App_CanTx_Create(
            send_non_periodic_msg_BMS_STARTUP,
            send_non_periodic_msg_BMS_WATCHDOG_TIMEOUT);
        can_rx_interface = App_CanRx_Create();
        imd = App_Imd_Create(can_tx_interface);
        world = App_BmsWorld_Create(can_tx_interface, can_rx_interface);

        // Default to starting the state machine in the `init` state
        state_machine =
            App_SharedStateMachine_Create(world, App_GetInitState());

        // Reset fake functions
        RESET_FAKE(send_non_periodic_msg_BMS_STARTUP);
        RESET_FAKE(send_non_periodic_msg_BMS_WATCHDOG_TIMEOUT);
    }

    void SetInitialState(const struct State *const initial_state)
    {
        App_SharedStateMachine_Destroy(state_machine);
        state_machine = App_SharedStateMachine_Create(world, initial_state);
        EXPECT_TRUE(state_machine);
    }

    void TearDown() override
    {
        assert(state_machine != NULL);
        App_SharedStateMachine_Destroy(state_machine);
        assert(world != NULL);
        App_BmsWorld_Destroy(world);
        world         = NULL;
        state_machine = NULL;
    }

    struct StateMachine *     state_machine;
    struct World *            world;
    struct BmsCanTxInterface *can_tx_interface;
    struct BmsCanRxInterface *can_rx_interface;
    struct Imd* imd;
};

TEST_F(
    BmsStateMachineTest,
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

TEST_F(BmsStateMachineTest, check_init_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetInitState());

    EXPECT_EQ(
        CANMSGS_BMS_STATE_MACHINE_STATE_INIT_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

TEST_F(BmsStateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetDriveState());

    EXPECT_EQ(
        CANMSGS_BMS_STATE_MACHINE_STATE_DRIVE_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

TEST_F(BmsStateMachineTest, check_fault_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetFaultState());

    EXPECT_EQ(
        CANMSGS_BMS_STATE_MACHINE_STATE_FAULT_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

TEST_F(BmsStateMachineTest, check_charge_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetChargeState());

    EXPECT_EQ(
        CANMSGS_BMS_STATE_MACHINE_STATE_CHARGE_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}
