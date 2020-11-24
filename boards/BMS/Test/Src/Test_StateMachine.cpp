#include <math.h>
#include "Test_Bms.h"
#include "Test_Imd.h"
#include "Test_BaseStateMachineTest.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "App_SharedMacros.h"
#include "states/App_InitState.h"
#include "states/App_DriveState.h"
#include "states/App_FaultState.h"
#include "states/App_ChargeState.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "configs/App_ImdConfig.h"
#include "configs/App_AccumulatorConfigs.h"
#include "configs/App_AccumulatorThresholds.h"
}

namespace StateMachineTest
{
FAKE_VOID_FUNC(
    send_non_periodic_msg_BMS_STARTUP,
    const struct CanMsgs_bms_startup_t *);
FAKE_VOID_FUNC(
    send_non_periodic_msg_BMS_WATCHDOG_TIMEOUT,
    const struct CanMsgs_bms_watchdog_timeout_t *);
FAKE_VALUE_FUNC(float, get_pwm_frequency);
FAKE_VALUE_FUNC(float, get_pwm_duty_cycle);
FAKE_VALUE_FUNC(uint16_t, get_seconds_since_power_on);
FAKE_VALUE_FUNC(uint32_t, get_current_ms);
FAKE_VOID_FUNC(
    heartbeat_timeout_callback,
    enum HeartbeatOneHot,
    enum HeartbeatOneHot);
FAKE_VOID_FUNC(turn_on_red_led);
FAKE_VOID_FUNC(turn_on_green_led);
FAKE_VOID_FUNC(turn_on_blue_led);
FAKE_VOID_FUNC(enable_charger);
FAKE_VOID_FUNC(disable_charger);
FAKE_VALUE_FUNC(bool, is_charger_connected);
FAKE_VALUE_FUNC(ExitCode, enable_bms_ok);
FAKE_VALUE_FUNC(ExitCode, disable_bms_ok);
FAKE_VALUE_FUNC(bool, is_bms_ok_enabled);
FAKE_VALUE_FUNC(ExitCode, enable_imd_ok);
FAKE_VALUE_FUNC(ExitCode, disable_imd_ok);
FAKE_VALUE_FUNC(bool, is_imd_ok_enabled);
FAKE_VALUE_FUNC(ExitCode, enable_bspd_ok);
FAKE_VALUE_FUNC(ExitCode, disable_bspd_ok);
FAKE_VALUE_FUNC(bool, is_bspd_ok_enabled);
FAKE_VALUE_FUNC(ExitCode, configure_daisy_chain);
FAKE_VALUE_FUNC(ExitCode, read_cell_voltages);
FAKE_VALUE_FUNC(float, get_min_cell_voltage);
FAKE_VALUE_FUNC(float, get_max_cell_voltage);
FAKE_VALUE_FUNC(float, get_average_cell_voltage);
FAKE_VALUE_FUNC(float, get_pack_voltage);
FAKE_VALUE_FUNC(float, get_segment_0_voltage);
FAKE_VALUE_FUNC(float, get_segment_1_voltage);
FAKE_VALUE_FUNC(float, get_segment_2_voltage);
FAKE_VALUE_FUNC(float, get_segment_3_voltage);
FAKE_VALUE_FUNC(float, get_segment_4_voltage);
FAKE_VALUE_FUNC(float, get_segment_5_voltage);
FAKE_VALUE_FUNC(ExitCode, read_cell_temperatures);
FAKE_VALUE_FUNC(float, get_min_cell_temperature);
FAKE_VALUE_FUNC(float, get_max_cell_temperature);
FAKE_VALUE_FUNC(float, get_average_cell_temperature);
FAKE_VALUE_FUNC(bool, is_air_negative_on);
FAKE_VALUE_FUNC(bool, is_air_positive_on);

class BmsStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        can_tx_interface = App_CanTx_Create(
            send_non_periodic_msg_BMS_STARTUP,
            send_non_periodic_msg_BMS_WATCHDOG_TIMEOUT);

        can_rx_interface = App_CanRx_Create();

        imd = App_Imd_Create(
            get_pwm_frequency, IMD_FREQUENCY_TOLERANCE, get_pwm_duty_cycle,
            get_seconds_since_power_on);

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS,
            HEARTBEAT_MONITOR_BOARDS_TO_CHECK, heartbeat_timeout_callback);

        rgb_led_sequence = App_SharedRgbLedSequence_Create(
            turn_on_red_led, turn_on_green_led, turn_on_blue_led);

        charger = App_Charger_Create(
            enable_charger, disable_charger, is_charger_connected);

        bms_ok = App_OkStatus_Create(
            enable_bms_ok, disable_bms_ok, is_bms_ok_enabled);

        imd_ok = App_OkStatus_Create(
            enable_imd_ok, disable_imd_ok, is_imd_ok_enabled);

        bspd_ok = App_OkStatus_Create(
            enable_bspd_ok, disable_bspd_ok, is_bspd_ok_enabled);

        accumulator = App_Accumulator_Create(
            configure_daisy_chain, read_cell_voltages, read_cell_temperatures,
            get_min_cell_voltage, get_max_cell_voltage,
            get_average_cell_voltage, get_pack_voltage, get_segment_0_voltage,
            get_segment_1_voltage, get_segment_2_voltage, get_segment_3_voltage,
            get_segment_4_voltage, get_segment_5_voltage,
            get_min_cell_temperature, get_max_cell_temperature,
            get_average_cell_temperature, MIN_CELL_VOLTAGE, MAX_CELL_VOLTAGE,
            MIN_SEGMENT_VOLTAGE, MAX_SEGMENT_VOLTAGE, MIN_PACK_VOLTAGE,
            MAX_PACK_VOLTAGE, DEFAULT_MIN_CELL_TEMPERATURE,
            DEFAULT_MAX_CELL_TEMPERATURE, CHARGE_STATE_MAX_CELL_TEMPERATURE);

        air_negative = App_SharedBinaryStatus_Create(is_air_negative_on);
        air_positive = App_SharedBinaryStatus_Create(is_air_positive_on);

        clock = App_SharedClock_Create();

        world = App_BmsWorld_Create(
            can_tx_interface, can_rx_interface, imd, heartbeat_monitor,
            rgb_led_sequence, charger, bms_ok, imd_ok, bspd_ok, accumulator,
            air_negative, air_positive, clock);

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
        RESET_FAKE(turn_on_red_led);
        RESET_FAKE(turn_on_green_led);
        RESET_FAKE(turn_on_blue_led);
        RESET_FAKE(enable_charger);
        RESET_FAKE(disable_charger);
        RESET_FAKE(enable_bms_ok);
        RESET_FAKE(disable_bms_ok);
        RESET_FAKE(is_bms_ok_enabled);
        RESET_FAKE(enable_imd_ok);
        RESET_FAKE(disable_imd_ok);
        RESET_FAKE(is_imd_ok_enabled);
        RESET_FAKE(enable_bspd_ok);
        RESET_FAKE(disable_bspd_ok);
        RESET_FAKE(is_bspd_ok_enabled);
        RESET_FAKE(configure_daisy_chain);
        RESET_FAKE(read_cell_voltages);
        RESET_FAKE(get_average_cell_voltage);
        RESET_FAKE(get_pack_voltage);
        RESET_FAKE(get_segment_0_voltage);
        RESET_FAKE(get_segment_1_voltage);
        RESET_FAKE(get_segment_2_voltage);
        RESET_FAKE(get_segment_3_voltage);
        RESET_FAKE(get_segment_4_voltage);
        RESET_FAKE(get_segment_5_voltage);
        RESET_FAKE(read_cell_temperatures);
        RESET_FAKE(get_min_cell_temperature);
        RESET_FAKE(get_max_cell_temperature);
        RESET_FAKE(get_average_cell_temperature);
        RESET_FAKE(is_air_negative_on);
        RESET_FAKE(is_air_positive_on);

        is_charger_connected_fake.return_val = true;
        get_min_cell_voltage_fake.return_val = 4.0f;
        get_max_cell_voltage_fake.return_val = 4.0f;
    }

    void TearDown() override
    {
        TearDownObject(world, App_BmsWorld_Destroy);
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        TearDownObject(can_tx_interface, App_CanTx_Destroy);
        TearDownObject(can_rx_interface, App_CanRx_Destroy);
        TearDownObject(imd, App_Imd_Destroy);
        TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy);
        TearDownObject(rgb_led_sequence, App_SharedRgbLedSequence_Destroy);
        TearDownObject(charger, App_Charger_Destroy);
        TearDownObject(bms_ok, App_OkStatus_Destroy);
        TearDownObject(imd_ok, App_OkStatus_Destroy);
        TearDownObject(bspd_ok, App_OkStatus_Destroy);
        TearDownObject(accumulator, App_Accumulator_Destroy);
        TearDownObject(air_negative, App_SharedBinaryStatus_Destroy);
        TearDownObject(air_positive, App_SharedBinaryStatus_Destroy);
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

    std::vector<const struct State *> GetAllStates(void)
    {
        return std::vector<const struct State *>{
            App_GetInitState(),
            App_GetDriveState(),
            App_GetChargeState(),
            App_GetFaultState(),
        };
    }

    void CheckInRangeCanSignalsInGivenState(
        const struct State *state,
        float               min_value,
        float               max_value,
        float &             fake_value,
        float (*value_can_signal_getter)(const struct BmsCanTxInterface *),
        uint8_t (*out_of_range_can_signal_getter)(
            const struct BmsCanTxInterface *),
        uint8_t  ok_choice,
        uint8_t  underflow_choice,
        uint8_t  overflow_choice,
        uint32_t task_period_ms)
    {
        // Normal range
        SetInitialState(state);
        fake_value = (min_value + max_value) / 2;
        LetTimePass(state_machine, task_period_ms);
        ASSERT_EQ(fake_value, value_can_signal_getter(can_tx_interface));
        ASSERT_EQ(ok_choice, out_of_range_can_signal_getter(can_tx_interface));

        // Underflow range
        SetInitialState(state);
        fake_value =
            std::nextafter(min_value, std::numeric_limits<float>::lowest());
        LetTimePass(state_machine, task_period_ms);
        ASSERT_EQ(
            underflow_choice, out_of_range_can_signal_getter(can_tx_interface));

        // Overflow range
        SetInitialState(state);
        fake_value =
            std::nextafter(max_value, std::numeric_limits<float>::max());
        LetTimePass(state_machine, task_period_ms);
        ASSERT_EQ(
            overflow_choice, out_of_range_can_signal_getter(can_tx_interface));
    }

    void UpdateClock(
        struct StateMachine *state_machine,
        uint32_t             current_time_ms) override
    {
        struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
        struct Clock *   clock = App_BmsWorld_GetClock(world);
        App_SharedClock_SetCurrentTimeInMilliseconds(clock, current_time_ms);
    }

    void UpdateSignals(
        struct StateMachine *state_machine,
        uint32_t             current_time_ms) override
    {
        // BMS doesn't use any signals currently
        UNUSED(state_machine);
        UNUSED(current_time_ms);
    }

    struct World *            world;
    struct StateMachine *     state_machine;
    struct BmsCanTxInterface *can_tx_interface;
    struct BmsCanRxInterface *can_rx_interface;
    struct Imd *              imd;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct RgbLedSequence *   rgb_led_sequence;
    struct Charger *          charger;
    struct OkStatus *         bms_ok;
    struct OkStatus *         imd_ok;
    struct OkStatus *         bspd_ok;
    struct Accumulator *      accumulator;
    struct BinaryStatus *     air_negative;
    struct BinaryStatus *     air_positive;
    struct Clock *            clock;

    const uint32_t TASK_1HZ_DURATION_MS   = 1000;
    const uint32_t TASK_100HZ_DURATION_MS = 10;
};

// BMS-31
TEST_F(BmsStateMachineTest, check_init_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetInitState());

    EXPECT_EQ(
        CANMSGS_BMS_STATE_MACHINE_STATE_INIT_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

// BMS-31
TEST_F(BmsStateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetDriveState());

    EXPECT_EQ(
        CANMSGS_BMS_STATE_MACHINE_STATE_DRIVE_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

// BMS-31
TEST_F(BmsStateMachineTest, check_fault_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetFaultState());

    EXPECT_EQ(
        CANMSGS_BMS_STATE_MACHINE_STATE_FAULT_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

// BMS-31
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
        LetTimePass(state_machine, 10);

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
        LetTimePass(state_machine, 10);

        EXPECT_EQ(
            fake_duty_cycle,
            App_CanTx_GetPeriodicSignal_DUTY_CYCLE(can_tx_interface));

        // To avoid false positives, we use a different frequency each time
        fake_duty_cycle++;
    }
}

// BMS-36
TEST_F(
    BmsStateMachineTest,
    check_imd_insulation_resistance_10hz_is_broadcasted_over_can_in_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);
        ImdTest::SetImdCondition(
            imd, IMD_NORMAL, get_pwm_frequency_fake.return_val);

        // Test an arbitrarily chosen valid resistance
        get_pwm_duty_cycle_fake.return_val = 50.0f;
        LetTimePass(state_machine, 10);
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
        LetTimePass(state_machine, 10);
        EXPECT_EQ(
            IMD_NORMAL,
            App_CanTx_GetPeriodicSignal_CONDITION(can_tx_interface));
        EXPECT_EQ(
            false,
            App_CanTx_GetPeriodicSignal_VALID_DUTY_CYCLE(can_tx_interface));
    }
}

// BMS-36
TEST_F(
    BmsStateMachineTest,
    check_imd_insulation_resistance_20hz_is_broadcasted_over_can_in_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);
        ImdTest::SetImdCondition(
            imd, IMD_UNDERVOLTAGE_DETECTED, get_pwm_frequency_fake.return_val);

        // Test an arbitrarily chosen valid resistance
        get_pwm_duty_cycle_fake.return_val = 50.0f;
        LetTimePass(state_machine, 10);
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
        LetTimePass(state_machine, 10);
        EXPECT_EQ(
            IMD_UNDERVOLTAGE_DETECTED,
            App_CanTx_GetPeriodicSignal_CONDITION(can_tx_interface));
        EXPECT_EQ(
            false,
            App_CanTx_GetPeriodicSignal_VALID_DUTY_CYCLE(can_tx_interface));
    }
}

// BMS-36
TEST_F(
    BmsStateMachineTest,
    check_imd_speed_start_status_30hz_is_broadcasted_over_can_in_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);
        ImdTest::SetImdCondition(
            imd, IMD_SST, get_pwm_frequency_fake.return_val);

        // Test an arbitrarily chosen SST_GOOD
        get_pwm_duty_cycle_fake.return_val = 7.5f;
        LetTimePass(state_machine, 10);
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
        LetTimePass(state_machine, 10);
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
        LetTimePass(state_machine, 10);
        EXPECT_EQ(
            IMD_SST, App_CanTx_GetPeriodicSignal_CONDITION(can_tx_interface));
        EXPECT_EQ(
            false,
            App_CanTx_GetPeriodicSignal_VALID_DUTY_CYCLE(can_tx_interface));
    }
}

// BMS-36
TEST_F(
    BmsStateMachineTest,
    check_imd_seconds_since_power_on_is_broadcasted_over_can_in_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);
        get_seconds_since_power_on_fake.return_val = 123;
        LetTimePass(state_machine, 10);
        EXPECT_EQ(
            123, App_CanTx_GetPeriodicSignal_SECONDS_SINCE_POWER_ON(
                     can_tx_interface));
    }
}

TEST_F(BmsStateMachineTest, rgb_led_sequence_in_all_states)
{
    unsigned int *call_counts[] = { &turn_on_red_led_fake.call_count,
                                    &turn_on_green_led_fake.call_count,
                                    &turn_on_blue_led_fake.call_count };

    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        RESET_FAKE(turn_on_red_led);
        RESET_FAKE(turn_on_green_led);
        RESET_FAKE(turn_on_blue_led);

        // Verify that we cycle through red, green, blue, red etc. for 99 times.
        // The number 99 can be changed to any other number that is a multiple
        // of 3. The significance of 3 is because we have 3 colors (Red, green,
        // and blue).
        for (size_t i = 0; i < 99; i++)
        {
            LetTimePass(state_machine, 1000);
            ASSERT_EQ(*call_counts[i % 3], i / 3 + 1);
        }
    }
}

// BMS-9
TEST_F(BmsStateMachineTest, charger_connection_status_in_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        is_charger_connected_fake.return_val = true;
        LetTimePass(state_machine, 1000);
        ASSERT_EQ(
            true, App_CanTx_GetPeriodicSignal_IS_CONNECTED(can_tx_interface));

        is_charger_connected_fake.return_val = false;
        LetTimePass(state_machine, 1000);
        ASSERT_EQ(
            false, App_CanTx_GetPeriodicSignal_IS_CONNECTED(can_tx_interface));
    }
}

// BMS-37
TEST_F(BmsStateMachineTest, check_bms_ok_is_broadcasted_over_can_in_all_states)
{
    // Enable BMS_OK
    is_bms_ok_enabled_fake.return_val = true;

    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        // Make sure the state machine sets the CAN signal for BMS_OK
        LetTimePass(state_machine, 10);
        ASSERT_EQ(true, App_CanTx_GetPeriodicSignal_BMS_OK(can_tx_interface));

        // Reset the CAN signal for BMS_OK
        App_CanTx_SetPeriodicSignal_BMS_OK(can_tx_interface, false);
    }
}

// BMS-37
TEST_F(BmsStateMachineTest, check_imd_ok_is_broadcasted_over_can_in_all_states)
{
    // Enable IMD_OK
    is_imd_ok_enabled_fake.return_val = true;

    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        // Make sure the state machine sets the CAN signal for IMD_OK
        LetTimePass(state_machine, 10);
        ASSERT_EQ(true, App_CanTx_GetPeriodicSignal_IMD_OK(can_tx_interface));

        // Reset the CAN signal for IMD_OK
        App_CanTx_SetPeriodicSignal_IMD_OK(can_tx_interface, false);
    }
}

// BMS-37
TEST_F(BmsStateMachineTest, check_bspd_ok_is_broadcasted_over_can_in_all_states)
{
    // Enable BSPD_OK
    is_bspd_ok_enabled_fake.return_val = true;

    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        // Make sure the state machine sets the CAN signal for BSPD_OK
        LetTimePass(state_machine, 10);
        ASSERT_EQ(true, App_CanTx_GetPeriodicSignal_BSPD_OK(can_tx_interface));

        // Reset the CAN signal for BSPD_OK
        App_CanTx_SetPeriodicSignal_BSPD_OK(can_tx_interface, false);
    }
}

// BMS-20
TEST_F(BmsStateMachineTest, charger_disconnects_in_charge_state)
{
    SetInitialState(App_GetChargeState());

    is_charger_connected_fake.return_val = false;

    LetTimePass(state_machine, 10);

    ASSERT_EQ(
        true, App_CanTx_GetPeriodicSignal_CHARGER_DISCONNECTED_IN_CHARGE_STATE(
                  can_tx_interface));
    ASSERT_EQ(
        App_GetFaultState(),
        App_SharedStateMachine_GetCurrentState(state_machine));
}

// BMS-38
TEST_F(BmsStateMachineTest, check_airs_can_signals_for_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        is_air_negative_on_fake.return_val = false;
        is_air_positive_on_fake.return_val = false;
        LetTimePass(state_machine, 10);
        ASSERT_EQ(
            false, App_CanTx_GetPeriodicSignal_AIR_NEGATIVE(can_tx_interface));
        ASSERT_EQ(
            false, App_CanTx_GetPeriodicSignal_AIR_POSITIVE(can_tx_interface));

        is_air_negative_on_fake.return_val = false;
        is_air_positive_on_fake.return_val = true;
        LetTimePass(state_machine, 10);
        ASSERT_EQ(
            false, App_CanTx_GetPeriodicSignal_AIR_NEGATIVE(can_tx_interface));
        ASSERT_EQ(
            true, App_CanTx_GetPeriodicSignal_AIR_POSITIVE(can_tx_interface));

        is_air_negative_on_fake.return_val = true;
        is_air_positive_on_fake.return_val = false;
        LetTimePass(state_machine, 10);
        ASSERT_EQ(
            true, App_CanTx_GetPeriodicSignal_AIR_NEGATIVE(can_tx_interface));
        ASSERT_EQ(
            false, App_CanTx_GetPeriodicSignal_AIR_POSITIVE(can_tx_interface));

        is_air_negative_on_fake.return_val = true;
        is_air_positive_on_fake.return_val = true;
        LetTimePass(state_machine, 10);
        ASSERT_EQ(
            true, App_CanTx_GetPeriodicSignal_AIR_NEGATIVE(can_tx_interface));
        ASSERT_EQ(
            true, App_CanTx_GetPeriodicSignal_AIR_POSITIVE(can_tx_interface));
    }
}

TEST_F(
    BmsStateMachineTest,
    check_accumulator_voltages_can_signals_in_all_states)
{
    for (const auto &state : GetAllStates())
    {
        CheckInRangeCanSignalsInGivenState(
            state, MIN_CELL_VOLTAGE, MAX_CELL_VOLTAGE,
            get_min_cell_voltage_fake.return_val,
            App_CanTx_GetPeriodicSignal_MIN_CELL_VOLTAGE,
            App_CanTx_GetPeriodicSignal_MIN_CELL_VOLTAGE_OUT_OF_RANGE,
            CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MIN_CELL_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
            CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MIN_CELL_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
            CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MIN_CELL_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE,
            TASK_100HZ_DURATION_MS);

        CheckInRangeCanSignalsInGivenState(
            state, MIN_CELL_VOLTAGE, MAX_CELL_VOLTAGE,
            get_max_cell_voltage_fake.return_val,
            App_CanTx_GetPeriodicSignal_MAX_CELL_VOLTAGE,
            App_CanTx_GetPeriodicSignal_MAX_CELL_VOLTAGE_OUT_OF_RANGE,
            CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MAX_CELL_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
            CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MAX_CELL_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
            CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MAX_CELL_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE,
            TASK_100HZ_DURATION_MS);

        CheckInRangeCanSignalsInGivenState(
            state, MIN_CELL_VOLTAGE, MAX_CELL_VOLTAGE,
            get_average_cell_voltage_fake.return_val,
            App_CanTx_GetPeriodicSignal_AVERAGE_CELL_VOLTAGE,
            App_CanTx_GetPeriodicSignal_AVERAGE_CELL_VOLTAGE_OUT_OF_RANGE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_AVERAGE_CELL_TEMP_OUT_OF_RANGE_OK_CHOICE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_AVERAGE_CELL_TEMP_OUT_OF_RANGE_UNDERFLOW_CHOICE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_AVERAGE_CELL_TEMP_OUT_OF_RANGE_OVERFLOW_CHOICE,
            TASK_100HZ_DURATION_MS);

        CheckInRangeCanSignalsInGivenState(
            state, MIN_PACK_VOLTAGE, MAX_PACK_VOLTAGE,
            get_pack_voltage_fake.return_val,
            App_CanTx_GetPeriodicSignal_PACK_VOLTAGE,
            App_CanTx_GetPeriodicSignal_PACK_VOLTAGE_OUT_OF_RANGE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_PACK_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_PACK_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_PACK_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE,
            TASK_100HZ_DURATION_MS);

        CheckInRangeCanSignalsInGivenState(
            state, MIN_SEGMENT_VOLTAGE, MAX_SEGMENT_VOLTAGE,
            get_segment_0_voltage_fake.return_val,
            App_CanTx_GetPeriodicSignal_SEGMENT_0_VOLTAGE,
            App_CanTx_GetPeriodicSignal_SEGMENT_0_VOLTAGE_OUT_OF_RANGE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_0_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_0_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_0_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE,
            TASK_100HZ_DURATION_MS);

        CheckInRangeCanSignalsInGivenState(
            state, MIN_SEGMENT_VOLTAGE, MAX_SEGMENT_VOLTAGE,
            get_segment_1_voltage_fake.return_val,
            App_CanTx_GetPeriodicSignal_SEGMENT_1_VOLTAGE,
            App_CanTx_GetPeriodicSignal_SEGMENT_1_VOLTAGE_OUT_OF_RANGE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_1_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_1_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_1_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE,
            TASK_100HZ_DURATION_MS);

        CheckInRangeCanSignalsInGivenState(
            state, MIN_SEGMENT_VOLTAGE, MAX_SEGMENT_VOLTAGE,
            get_segment_2_voltage_fake.return_val,
            App_CanTx_GetPeriodicSignal_SEGMENT_2_VOLTAGE,
            App_CanTx_GetPeriodicSignal_SEGMENT_2_VOLTAGE_OUT_OF_RANGE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_2_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_2_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_2_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE,
            TASK_100HZ_DURATION_MS);

        CheckInRangeCanSignalsInGivenState(
            state, MIN_SEGMENT_VOLTAGE, MAX_SEGMENT_VOLTAGE,
            get_segment_3_voltage_fake.return_val,
            App_CanTx_GetPeriodicSignal_SEGMENT_3_VOLTAGE,
            App_CanTx_GetPeriodicSignal_SEGMENT_3_VOLTAGE_OUT_OF_RANGE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_3_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_3_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_3_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE,
            TASK_100HZ_DURATION_MS);

        CheckInRangeCanSignalsInGivenState(
            state, MIN_SEGMENT_VOLTAGE, MAX_SEGMENT_VOLTAGE,
            get_segment_4_voltage_fake.return_val,
            App_CanTx_GetPeriodicSignal_SEGMENT_4_VOLTAGE,
            App_CanTx_GetPeriodicSignal_SEGMENT_4_VOLTAGE_OUT_OF_RANGE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_4_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_4_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_4_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE,
            TASK_100HZ_DURATION_MS);

        CheckInRangeCanSignalsInGivenState(
            state, MIN_SEGMENT_VOLTAGE, MAX_SEGMENT_VOLTAGE,
            get_segment_5_voltage_fake.return_val,
            App_CanTx_GetPeriodicSignal_SEGMENT_5_VOLTAGE,
            App_CanTx_GetPeriodicSignal_SEGMENT_5_VOLTAGE_OUT_OF_RANGE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_5_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_5_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_5_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE,
            TASK_100HZ_DURATION_MS);
    }
}

// BMS-5
TEST_F(
    BmsStateMachineTest,
    check_accumulator_temperature_can_signals_in_all_states)
{
    for (const auto &state : GetAllStates())
    {
        CheckInRangeCanSignalsInGivenState(
            state, DEFAULT_MIN_CELL_TEMPERATURE, DEFAULT_MAX_CELL_TEMPERATURE,
            get_average_cell_temperature_fake.return_val,
            App_CanTx_GetPeriodicSignal_AVERAGE_CELL_TEMP,
            App_CanTx_GetPeriodicSignal_AVERAGE_CELL_TEMP_OUT_OF_RANGE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_AVERAGE_CELL_TEMP_OUT_OF_RANGE_OK_CHOICE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_AVERAGE_CELL_TEMP_OUT_OF_RANGE_UNDERFLOW_CHOICE,
            CANMSGS_BMS_NON_CRITICAL_ERRORS_AVERAGE_CELL_TEMP_OUT_OF_RANGE_OVERFLOW_CHOICE,
            TASK_1HZ_DURATION_MS);
    }
}

// BMS-5, BMS-6, BMS-7
TEST_F(
    BmsStateMachineTest,
    check_max_cell_temperature_can_signals_in_all_states)
{
    for (const auto &state : GetAllStates())
    {
        if (state != App_GetChargeState())
        {
            CheckInRangeCanSignalsInGivenState(
                state, DEFAULT_MIN_CELL_TEMPERATURE,
                DEFAULT_MAX_CELL_TEMPERATURE,
                get_min_cell_temperature_fake.return_val,
                App_CanTx_GetPeriodicSignal_MIN_CELL_TEMP,
                App_CanTx_GetPeriodicSignal_MIN_CELL_TEMP_OUT_OF_RANGE,
                CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MIN_CELL_TEMP_OUT_OF_RANGE_OK_CHOICE,
                CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MIN_CELL_TEMP_OUT_OF_RANGE_UNDERFLOW_CHOICE,
                CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MIN_CELL_TEMP_OUT_OF_RANGE_OVERFLOW_CHOICE,
                TASK_1HZ_DURATION_MS);

            CheckInRangeCanSignalsInGivenState(
                state, DEFAULT_MIN_CELL_TEMPERATURE,
                DEFAULT_MAX_CELL_TEMPERATURE,
                get_max_cell_temperature_fake.return_val,
                App_CanTx_GetPeriodicSignal_MAX_CELL_TEMP,
                App_CanTx_GetPeriodicSignal_MAX_CELL_TEMP_OUT_OF_RANGE,
                CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MAX_CELL_TEMP_OUT_OF_RANGE_OK_CHOICE,
                CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MAX_CELL_TEMP_OUT_OF_RANGE_UNDERFLOW_CHOICE,
                CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MAX_CELL_TEMP_OUT_OF_RANGE_OVERFLOW_CHOICE,
                TASK_1HZ_DURATION_MS);
        }
        else
        {
            const float fake_min_cell_temperature = nextafter(
                DEFAULT_MIN_CELL_TEMPERATURE,
                std::numeric_limits<float>::lowest());
            const float fake_average_cell_temperature =
                (DEFAULT_MIN_CELL_TEMPERATURE +
                 CHARGE_STATE_MAX_CELL_TEMPERATURE) /
                2.0;
            const float fake_max_cell_temperature = std::nextafter(
                CHARGE_STATE_MAX_CELL_TEMPERATURE,
                std::numeric_limits<float>::max());

            SetInitialState(state);
            get_min_cell_temperature_fake.return_val =
                fake_min_cell_temperature;
            get_max_cell_temperature_fake.return_val =
                fake_min_cell_temperature;
            LetTimePass(state_machine, TASK_1HZ_DURATION_MS);
            ASSERT_EQ(
                CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MIN_CELL_TEMP_OUT_OF_RANGE_UNDERFLOW_CHOICE,
                App_CanTx_GetPeriodicSignal_MIN_CELL_TEMP_OUT_OF_RANGE(
                    can_tx_interface));
            ASSERT_EQ(
                CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MAX_CELL_TEMP_OUT_OF_RANGE_UNDERFLOW_CHOICE,
                App_CanTx_GetPeriodicSignal_MAX_CELL_TEMP_OUT_OF_RANGE(
                    can_tx_interface));
            ASSERT_EQ(
                CANMSGS_BMS_STATE_MACHINE_STATE_FAULT_CHOICE,
                App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));

            SetInitialState(state);
            get_min_cell_temperature_fake.return_val =
                fake_average_cell_temperature;
            get_max_cell_temperature_fake.return_val =
                fake_average_cell_temperature;
            LetTimePass(state_machine, TASK_1HZ_DURATION_MS);
            ASSERT_EQ(
                CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MIN_CELL_TEMP_OUT_OF_RANGE_OK_CHOICE,
                App_CanTx_GetPeriodicSignal_MIN_CELL_TEMP_OUT_OF_RANGE(
                    can_tx_interface));
            ASSERT_EQ(
                CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MAX_CELL_TEMP_OUT_OF_RANGE_OK_CHOICE,
                App_CanTx_GetPeriodicSignal_MAX_CELL_TEMP_OUT_OF_RANGE(
                    can_tx_interface));
            ASSERT_EQ(
                CANMSGS_BMS_STATE_MACHINE_STATE_CHARGE_CHOICE,
                App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));

            SetInitialState(state);
            get_min_cell_temperature_fake.return_val =
                fake_min_cell_temperature;
            get_max_cell_temperature_fake.return_val =
                fake_average_cell_temperature;
            LetTimePass(state_machine, TASK_1HZ_DURATION_MS);
            ASSERT_EQ(
                CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MIN_CELL_TEMP_OUT_OF_RANGE_UNDERFLOW_CHOICE,
                App_CanTx_GetPeriodicSignal_MIN_CELL_TEMP_OUT_OF_RANGE(
                    can_tx_interface));
            ASSERT_EQ(
                CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MAX_CELL_TEMP_OUT_OF_RANGE_OK_CHOICE,
                App_CanTx_GetPeriodicSignal_MAX_CELL_TEMP_OUT_OF_RANGE(
                    can_tx_interface));
            ASSERT_EQ(
                CANMSGS_BMS_STATE_MACHINE_STATE_FAULT_CHOICE,
                App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));

            SetInitialState(state);
            get_min_cell_temperature_fake.return_val =
                fake_average_cell_temperature;
            get_max_cell_temperature_fake.return_val =
                fake_max_cell_temperature;
            LetTimePass(state_machine, TASK_1HZ_DURATION_MS);
            ASSERT_EQ(
                CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MIN_CELL_TEMP_OUT_OF_RANGE_OK_CHOICE,
                App_CanTx_GetPeriodicSignal_MIN_CELL_TEMP_OUT_OF_RANGE(
                    can_tx_interface));
            ASSERT_EQ(
                CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MAX_CELL_TEMP_OUT_OF_RANGE_OVERFLOW_CHOICE,
                App_CanTx_GetPeriodicSignal_MAX_CELL_TEMP_OUT_OF_RANGE(
                    can_tx_interface));
            ASSERT_EQ(
                CANMSGS_BMS_STATE_MACHINE_STATE_FAULT_CHOICE,
                App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));

            SetInitialState(state);
            get_min_cell_temperature_fake.return_val =
                fake_max_cell_temperature;
            get_max_cell_temperature_fake.return_val =
                fake_max_cell_temperature;
            LetTimePass(state_machine, TASK_1HZ_DURATION_MS);
            ASSERT_EQ(
                CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MIN_CELL_TEMP_OUT_OF_RANGE_OVERFLOW_CHOICE,
                App_CanTx_GetPeriodicSignal_MIN_CELL_TEMP_OUT_OF_RANGE(
                    can_tx_interface));
            ASSERT_EQ(
                CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MAX_CELL_TEMP_OUT_OF_RANGE_OVERFLOW_CHOICE,
                App_CanTx_GetPeriodicSignal_MAX_CELL_TEMP_OUT_OF_RANGE(
                    can_tx_interface));
            ASSERT_EQ(
                CANMSGS_BMS_STATE_MACHINE_STATE_FAULT_CHOICE,
                App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
        }
    }
}

} // namespace StateMachineTest
