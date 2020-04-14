#include "fff.h"
#include "gtest/gtest.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
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

    FAKE_VALUE_FUNC(float, get_pwm_frequency);
    FAKE_VALUE_FUNC(float, get_pwm_frequency_tolerance);
    FAKE_VALUE_FUNC(float, get_pwm_duty_cycle);
    FAKE_VALUE_FUNC(uint32_t, get_seconds_since_power_on);
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
            can_tx_interface, get_pwm_frequency, get_pwm_duty_cycle,
            get_seconds_since_power_on);
        world = App_BmsWorld_Create(can_tx_interface, can_rx_interface, imd);

        // Default to starting the state machine in the `init` state
        state_machine =
            App_SharedStateMachine_Create(world, App_GetInitState());

        // Reset fake functions
        RESET_FAKE(send_non_periodic_msg_BMS_STARTUP);
        RESET_FAKE(send_non_periodic_msg_BMS_WATCHDOG_TIMEOUT);
        RESET_FAKE(get_pwm_frequency);
        RESET_FAKE(get_pwm_frequency_tolerance);
        RESET_FAKE(get_pwm_duty_cycle);
        RESET_FAKE(get_seconds_since_power_on);
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
        ASSERT_TRUE(imd != NULL);
        ASSERT_TRUE(state_machine != NULL);

        App_BmsWorld_Destroy(world);
        App_CanTx_Destroy(can_tx_interface);
        App_CanRx_Destroy(can_rx_interface);
        App_Imd_Destroy(imd);
        App_SharedStateMachine_Destroy(state_machine);

        world            = NULL;
        can_tx_interface = NULL;
        can_rx_interface = NULL;
        imd              = NULL;
        state_machine    = NULL;
    }

    struct World *            world;
    struct BmsCanTxInterface *can_tx_interface;
    struct BmsCanRxInterface *can_rx_interface;
    struct Imd *              imd;
    struct StateMachine *     state_machine;
};

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
