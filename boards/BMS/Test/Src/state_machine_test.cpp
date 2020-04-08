#include "fff.h"
#include "imd_fff.h"
#include "gtest/gtest.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "states/App_InitState.h"
#include "states/App_DriveState.h"

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
        imd              = App_Imd_Create(
            get_pwm_frequency, get_pwm_frequency_tolerance, get_pwm_duty_cycle,
            get_seconds_since_power_on);
        world = App_BmsWorld_Create(can_tx_interface, can_rx_interface, imd);

        // Default to starting the state machine in the `init` state
        state_machine =
            App_SharedStateMachine_Create(world, App_GetInitState());

        RESET_FAKE(send_non_periodic_msg_BMS_STARTUP);
        RESET_FAKE(send_non_periodic_msg_BMS_WATCHDOG_TIMEOUT);
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
    struct Imd *              imd;
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
