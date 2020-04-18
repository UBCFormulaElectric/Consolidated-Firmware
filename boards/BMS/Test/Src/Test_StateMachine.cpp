#include "Test_StateMachine.h"
#include "Test_Imd.h"

extern "C"
{
#include "App_SharedHeartbeatMonitor.h"
}

DEFINE_FAKE_VOID_FUNC(
    send_non_periodic_msg_BMS_STARTUP,
    struct CanMsgs_bms_startup_t *);
DEFINE_FAKE_VOID_FUNC(
    send_non_periodic_msg_BMS_WATCHDOG_TIMEOUT,
    struct CanMsgs_bms_watchdog_timeout_t *);
FAKE_VALUE_FUNC(uint32_t, get_current_ms);
FAKE_VOID_FUNC(
    heartbeat_timeout_callback,
    enum HeartbeatOneHot,
    enum HeartbeatOneHot);

class BmsStateMachineTest : public BmsTest
{
  protected:
    void SetUp() override
    {
        constexpr uint32_t DEFAULT_HEARTBEAT_TIMEOUT_PERIOD_MS = 500U;
        constexpr enum HeartbeatOneHot DEFAULT_HEARTBEAT_BOARDS_TO_CHECK =
            (enum HeartbeatOneHot)(
                FSM_HEARTBEAT_ONE_HOT | DCM_HEARTBEAT_ONE_HOT |
                PDM_HEARTBEAT_ONE_HOT);

        can_tx_interface = App_CanTx_Create(
            send_non_periodic_msg_BMS_STARTUP,
            send_non_periodic_msg_BMS_WATCHDOG_TIMEOUT);
        can_rx_interface  = App_CanRx_Create();
        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, DEFAULT_HEARTBEAT_TIMEOUT_PERIOD_MS,
            DEFAULT_HEARTBEAT_BOARDS_TO_CHECK, heartbeat_timeout_callback);
        imd = App_Imd_Create(
            get_pwm_frequency, 2.0f, get_pwm_duty_cycle,
            get_seconds_since_power_on);
        world = App_BmsWorld_Create(
            can_tx_interface, can_rx_interface, imd, heartbeat_monitor);

        // Default to starting the state machine in the `init` state
        state_machine =
            App_SharedStateMachine_Create(world, App_GetInitState());

        RESET_FAKE(send_non_periodic_msg_BMS_STARTUP);
        RESET_FAKE(send_non_periodic_msg_BMS_WATCHDOG_TIMEOUT);
        RESET_FAKE(get_pwm_frequency);
        RESET_FAKE(get_pwm_duty_cycle);
        RESET_FAKE(get_seconds_since_power_on);
        RESET_FAKE(get_current_ms);
        RESET_FAKE(heartbeat_timeout_callback);
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

    std::vector<const struct State *> GetAllStates(void)
    {
        return std::vector<const struct State *>{
            App_GetInitState(),
            App_GetDriveState(),
            App_GetChargeState(),
            App_GetFaultState(),
        };
    }

    void TearDown() override
    {
        ASSERT_TRUE(world != NULL);
        ASSERT_TRUE(can_tx_interface != NULL);
        ASSERT_TRUE(can_rx_interface != NULL);
        ASSERT_TRUE(imd != NULL);
        ASSERT_TRUE(state_machine != NULL);
        ASSERT_TRUE(heartbeat_monitor != NULL);

        App_BmsWorld_Destroy(world);
        App_CanTx_Destroy(can_tx_interface);
        App_CanRx_Destroy(can_rx_interface);
        App_Imd_Destroy(imd);
        App_SharedStateMachine_Destroy(state_machine);
        App_SharedHeartbeatMonitor_Destroy(heartbeat_monitor);

        world             = NULL;
        can_tx_interface  = NULL;
        can_rx_interface  = NULL;
        imd               = NULL;
        state_machine     = NULL;
        heartbeat_monitor = NULL;
    }

    struct World *            world;
    struct BmsCanTxInterface *can_tx_interface;
    struct BmsCanRxInterface *can_rx_interface;
    struct StateMachine *     state_machine;
    struct HeartbeatMonitor * heartbeat_monitor;
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

TEST_F(
    BmsStateMachineTest,
    check_imd_frequency_is_broadcasted_over_can_in_all_states)
{
    float fake_frequency = 0.0f;

    for (const auto &state : GetAllStates())
    {
        SetInitialState(state);
        get_pwm_frequency_fake.return_val = fake_frequency;
        App_SharedStateMachine_Tick(state_machine);

        EXPECT_EQ(
            fake_frequency,
            App_CanTx_GetPeriodicSignal_FREQUENCY(can_tx_interface));

        // To avoid false positives, we use a different duty cycle each time
        fake_frequency++;
    }
}

TEST_F(
    BmsStateMachineTest,
    check_imd_duty_cycle_is_broadcasted_over_can_in_all_states)
{
    float fake_duty_cycle = 0.0f;

    for (const auto &state : GetAllStates())
    {
        SetInitialState(state);
        get_pwm_duty_cycle_fake.return_val = fake_duty_cycle;
        App_SharedStateMachine_Tick(state_machine);

        EXPECT_EQ(
            fake_duty_cycle,
            App_CanTx_GetPeriodicSignal_DUTY_CYCLE(can_tx_interface));

        // To avoid false positives, we use a different frequency each time
        fake_duty_cycle++;
    }
}

TEST_F(
    BmsStateMachineTest,
    check_imd_insulation_resistance_10hz_is_broadcasted_over_can_in_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);
        SetImdCondition(IMD_NORMAL, get_pwm_frequency_fake.return_val);

        // Test an arbitrarily chosen valid resistance
        get_pwm_duty_cycle_fake.return_val = 50.0f;
        App_SharedStateMachine_Tick(state_machine);
        EXPECT_EQ(
            IMD_NORMAL,
            App_CanTx_GetPeriodicSignal_CONDITION(can_tx_interface));
        EXPECT_EQ(
            true,
            App_CanTx_GetPeriodicSignal_VALID_DUTY_CYCLE(can_tx_interface));
        EXPECT_EQ(
            1200, App_CanTx_GetPeriodicSignal_INSULATION_MEASUREMENT_DCP_10_HZ(
                      can_tx_interface));

        // Test an arbitrarily chosen invalid resistance
        get_pwm_duty_cycle_fake.return_val = 0.0f;
        App_SharedStateMachine_Tick(state_machine);
        EXPECT_EQ(
            IMD_NORMAL,
            App_CanTx_GetPeriodicSignal_CONDITION(can_tx_interface));
        EXPECT_EQ(
            false,
            App_CanTx_GetPeriodicSignal_VALID_DUTY_CYCLE(can_tx_interface));
    }
}

TEST_F(
    BmsStateMachineTest,
    check_imd_insulation_resistance_20hz_is_broadcasted_over_can_in_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);
        SetImdCondition(
            IMD_UNDERVOLTAGE_DETECTED, get_pwm_frequency_fake.return_val);

        // Test an arbitrarily chosen valid resistance
        get_pwm_duty_cycle_fake.return_val = 50.0f;
        App_SharedStateMachine_Tick(state_machine);
        EXPECT_EQ(
            IMD_UNDERVOLTAGE_DETECTED,
            App_CanTx_GetPeriodicSignal_CONDITION(can_tx_interface));
        EXPECT_EQ(
            true,
            App_CanTx_GetPeriodicSignal_VALID_DUTY_CYCLE(can_tx_interface));
        EXPECT_EQ(
            1200, App_CanTx_GetPeriodicSignal_INSULATION_MEASUREMENT_DCP_20_HZ(
                      can_tx_interface));

        // Test an arbitrarily chosen invalid resistance
        get_pwm_duty_cycle_fake.return_val = 0.0f;
        App_SharedStateMachine_Tick(state_machine);
        EXPECT_EQ(
            IMD_UNDERVOLTAGE_DETECTED,
            App_CanTx_GetPeriodicSignal_CONDITION(can_tx_interface));
        EXPECT_EQ(
            false,
            App_CanTx_GetPeriodicSignal_VALID_DUTY_CYCLE(can_tx_interface));
    }
}

TEST_F(
    BmsStateMachineTest,
    check_imd_speed_start_status_30hz_is_broadcasted_over_can_in_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);
        SetImdCondition(IMD_SST, get_pwm_frequency_fake.return_val);

        // Test an arbitrarily chosen SST_GOOD
        get_pwm_duty_cycle_fake.return_val = 7.5f;
        App_SharedStateMachine_Tick(state_machine);
        EXPECT_EQ(
            IMD_SST, App_CanTx_GetPeriodicSignal_CONDITION(can_tx_interface));
        EXPECT_EQ(
            true,
            App_CanTx_GetPeriodicSignal_VALID_DUTY_CYCLE(can_tx_interface));
        EXPECT_EQ(
            SST_GOOD, App_CanTx_GetPeriodicSignal_SPEED_START_STATUS_30_HZ(
                          can_tx_interface));

        // Test an arbitrarily chosen SST_BAD
        get_pwm_duty_cycle_fake.return_val = 92.5f;
        App_SharedStateMachine_Tick(state_machine);
        EXPECT_EQ(
            IMD_SST, App_CanTx_GetPeriodicSignal_CONDITION(can_tx_interface));
        EXPECT_EQ(
            true,
            App_CanTx_GetPeriodicSignal_VALID_DUTY_CYCLE(can_tx_interface));
        EXPECT_EQ(
            SST_BAD, App_CanTx_GetPeriodicSignal_SPEED_START_STATUS_30_HZ(
                         can_tx_interface));

        // Test an arbitrarily chosen invalid SST status
        get_pwm_duty_cycle_fake.return_val = 0.0f;
        App_SharedStateMachine_Tick(state_machine);
        EXPECT_EQ(
            IMD_SST, App_CanTx_GetPeriodicSignal_CONDITION(can_tx_interface));
        EXPECT_EQ(
            false,
            App_CanTx_GetPeriodicSignal_VALID_DUTY_CYCLE(can_tx_interface));
    }
}

TEST_F(
    BmsStateMachineTest,
    check_imd_seconds_since_power_on_is_broadcasted_over_can_in_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);
        get_seconds_since_power_on_fake.return_val = 123;
        App_SharedStateMachine_Tick(state_machine);
        EXPECT_EQ(
            123, App_CanTx_GetPeriodicSignal_SECONDS_SINCE_POWER_ON(
                     can_tx_interface));
    }
};
