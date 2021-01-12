#include <math.h>
#include "Test_Bms.h"
#include "Test_Imd.h"
#include "Test_BaseStateMachineTest.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "App_SharedMacros.h"
#include "states/App_InitState.h"
#include "states/App_AirOpenState.h"
#include "states/App_PreChargeState.h"
#include "states/App_DriveState.h"
#include "states/App_FaultState.h"
#include "states/App_ChargeState.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "configs/App_ImdConfig.h"
#include "configs/App_AccumulatorConfigs.h"
#include "configs/App_AccumulatorThresholds.h"
#include "configs/App_CellMonitorsThresholds.h"
#include "configs/App_PreChargeThreshold.h"
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
FAKE_VALUE_FUNC(bool, is_air_negative_closed);
FAKE_VALUE_FUNC(bool, is_air_positive_closed);
FAKE_VALUE_FUNC(ExitCode, read_die_temperatures);
FAKE_VALUE_FUNC(float, get_segment_0_die_temp);
FAKE_VALUE_FUNC(float, get_segment_1_die_temp);
FAKE_VALUE_FUNC(float, get_segment_2_die_temp);
FAKE_VALUE_FUNC(float, get_segment_3_die_temp);
FAKE_VALUE_FUNC(float, get_segment_4_die_temp);
FAKE_VALUE_FUNC(float, get_segment_5_die_temp);
FAKE_VALUE_FUNC(float, get_max_die_temp);
FAKE_VALUE_FUNC(bool, is_air_negative_on);
FAKE_VALUE_FUNC(bool, is_air_positive_on);
FAKE_VOID_FUNC(open_air_positive);
FAKE_VOID_FUNC(close_air_positive);
FAKE_VOID_FUNC(enable_pre_charge);
FAKE_VOID_FUNC(disable_pre_charge);
FAKE_VALUE_FUNC(float, get_raw_ts_voltage);
FAKE_VALUE_FUNC(float, get_ts_voltage, float);

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
            configure_daisy_chain, read_cell_voltages, get_min_cell_voltage,
            get_max_cell_voltage, get_average_cell_voltage, get_pack_voltage,
            get_segment_0_voltage, get_segment_1_voltage, get_segment_2_voltage,
            get_segment_3_voltage, get_segment_4_voltage, get_segment_5_voltage,
            MIN_CELL_VOLTAGE, MAX_CELL_VOLTAGE, MIN_SEGMENT_VOLTAGE,
            MAX_SEGMENT_VOLTAGE, MIN_PACK_VOLTAGE, MAX_PACK_VOLTAGE);

        cell_monitors = App_CellMonitors_Create(
            read_die_temperatures, get_segment_0_die_temp,
            get_segment_1_die_temp, get_segment_2_die_temp,
            get_segment_3_die_temp, get_segment_4_die_temp,
            get_segment_5_die_temp, get_max_die_temp, MIN_ITMP_DEGC,
            MAX_ITMP_DEGC, DIE_TEMP_TO_REENABLE_CHARGER_DEGC,
            DIE_TEMP_TO_REENABLE_CELL_BALANCING_DEGC,
            DIE_TEMP_TO_DISABLE_CELL_BALANCING_DEGC,
            DIE_TEMP_TO_DISABLE_CHARGER_DEGC);

        pre_charge = App_PreCharge_Create(
            enable_pre_charge, disable_pre_charge, get_raw_ts_voltage,
            get_ts_voltage, MIN_PRE_CHARGE_DURATION_MS,
            MIN_PRE_CHARGE_COMPLETE_DURATION_MS,
            MAX_PRE_CHARGE_COMPLETE_DURATION_MS);

        airs = App_Airs_Create(
            is_air_positive_closed, is_air_negative_closed, close_air_positive,
            open_air_positive);

        clock = App_SharedClock_Create();

        world = App_BmsWorld_Create(
            can_tx_interface, can_rx_interface, imd, heartbeat_monitor,
            rgb_led_sequence, charger, bms_ok, imd_ok, bspd_ok, accumulator,
            cell_monitors, airs, pre_charge, clock);

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
        RESET_FAKE(get_segment_0_die_temp);
        RESET_FAKE(get_segment_1_die_temp);
        RESET_FAKE(get_segment_2_die_temp);
        RESET_FAKE(get_segment_3_die_temp);
        RESET_FAKE(get_segment_4_die_temp);
        RESET_FAKE(get_segment_5_die_temp);
        RESET_FAKE(get_max_die_temp);
        RESET_FAKE(is_air_negative_closed);
        RESET_FAKE(is_air_positive_closed);

        // The charger is connected to prevent other tests from entering the
        // fault state from the charge state
        is_charger_connected_fake.return_val = true;

        // A voltage in [3.0, 4.2] was arbitrarily chosen to prevent other
        // tests from entering the fault state
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
        TearDownObject(cell_monitors, App_CellMonitors_Destroy);
        TearDownObject(airs, App_Airs_Destroy);
        TearDownObject(pre_charge, App_PreCharge_Destroy);
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
            App_GetInitState(),  App_GetAirOpenState(), App_GetPreChargeState(),
            App_GetDriveState(), App_GetChargeState(),  App_GetFaultState(),
        };
    }

    void CheckInRangeCanSignalsInGivenState(
        const State *state,
        float        min_value,
        float        max_value,
        float &      fake_value,
        float (*value_can_signal_getter)(const struct BmsCanTxInterface *),
        uint8_t (*out_of_range_can_signal_getter)(
            const struct BmsCanTxInterface *),
        uint8_t ok_choice,
        uint8_t underflow_choice,
        uint8_t overflow_choice)
    {
        SetInitialState(state);

        // Normal range
        fake_value = (min_value + max_value) / 2;
        LetTimePass(state_machine, 1000);
        ASSERT_EQ(fake_value, value_can_signal_getter(can_tx_interface));
        ASSERT_EQ(ok_choice, out_of_range_can_signal_getter(can_tx_interface));

        // Underflow range
        fake_value =
            std::nextafter(min_value, std::numeric_limits<float>::lowest());
        LetTimePass(state_machine, 1000);
        ASSERT_EQ(
            underflow_choice, out_of_range_can_signal_getter(can_tx_interface));

        // Overflow range
        fake_value =
            std::nextafter(max_value, std::numeric_limits<float>::max());
        LetTimePass(state_machine, 1000);
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
    struct CellMonitors *     cell_monitors;
    struct Airs *             airs;
    struct PreCharge *        pre_charge;
    struct Clock *            clock;
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

        is_air_negative_closed_fake.return_val = false;
        is_air_positive_closed_fake.return_val = false;
        LetTimePass(state_machine, 10);
        ASSERT_EQ(
            false, App_CanTx_GetPeriodicSignal_AIR_NEGATIVE(can_tx_interface));
        ASSERT_EQ(
            false, App_CanTx_GetPeriodicSignal_AIR_POSITIVE(can_tx_interface));

        is_air_negative_closed_fake.return_val = false;
        is_air_positive_closed_fake.return_val = true;
        LetTimePass(state_machine, 10);
        ASSERT_EQ(
            false, App_CanTx_GetPeriodicSignal_AIR_NEGATIVE(can_tx_interface));
        ASSERT_EQ(
            true, App_CanTx_GetPeriodicSignal_AIR_POSITIVE(can_tx_interface));

        is_air_negative_closed_fake.return_val = true;
        is_air_positive_closed_fake.return_val = false;
        LetTimePass(state_machine, 10);
        ASSERT_EQ(
            true, App_CanTx_GetPeriodicSignal_AIR_NEGATIVE(can_tx_interface));
        ASSERT_EQ(
            false, App_CanTx_GetPeriodicSignal_AIR_POSITIVE(can_tx_interface));

        is_air_negative_closed_fake.return_val = true;
        is_air_positive_closed_fake.return_val = true;
        LetTimePass(state_machine, 10);
        ASSERT_EQ(
            true, App_CanTx_GetPeriodicSignal_AIR_NEGATIVE(can_tx_interface));
        ASSERT_EQ(
            true, App_CanTx_GetPeriodicSignal_AIR_POSITIVE(can_tx_interface));
    }
}
// BMS-17
TEST_F(
    BmsStateMachineTest,
    charger_non_critical_fault_set_when_itmp_exceeds_charger_threshold)
{
    SetInitialState(App_GetChargeState());

    App_Charger_Enable(charger);

    // Die temperature underflow value
    const float fake_die_temp = 120.0;
    get_max_die_temp_fake.return_val =
        std::nextafter(fake_die_temp, std::numeric_limits<float>::lowest());
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(
        CANMSGS_BMS_NON_CRITICAL_ERRORS_ITMP_CHARGER_HAS_OVERFLOW_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_ITMP_CHARGER_HAS_OVERFLOW(
            can_tx_interface));
    ASSERT_EQ(true, App_Charger_IsEnabled(charger));

    // Die temperature overflow value
    get_max_die_temp_fake.return_val =
        std::nextafter(fake_die_temp, std::numeric_limits<float>::max());
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(
        CANMSGS_BMS_NON_CRITICAL_ERRORS_ITMP_CHARGER_HAS_OVERFLOW_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_ITMP_CHARGER_HAS_OVERFLOW(
            can_tx_interface));
    ASSERT_EQ(false, App_Charger_IsEnabled(charger));
}

// BMS-17
TEST_F(BmsStateMachineTest, charger_is_re_enabled_when_itmp_is_in_range)
{
    SetInitialState(App_GetChargeState());

    App_Charger_Enable(charger);

    // Die temperature overflow
    float fake_die_temp = 120.0;
    get_max_die_temp_fake.return_val =
        std::nextafter(fake_die_temp, std::numeric_limits<float>::max());
    LetTimePass(state_machine, 1000);

    // Die temperature overflow (slightly greater than the threshold to
    // re-enable the charger)
    fake_die_temp = 115.0;
    get_max_die_temp_fake.return_val =
        std::nextafter(fake_die_temp, std::numeric_limits<float>::max());
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(false, App_Charger_IsEnabled(charger));
    ASSERT_EQ(
        CANMSGS_BMS_NON_CRITICAL_ERRORS_ITMP_CHARGER_HAS_OVERFLOW_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_ITMP_CHARGER_HAS_OVERFLOW(
            can_tx_interface));

    // Die temperature underflow value
    get_max_die_temp_fake.return_val =
        std::nextafter(fake_die_temp, std::numeric_limits<float>::lowest());
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(true, App_Charger_IsEnabled(charger));
    ASSERT_EQ(
        CANMSGS_BMS_NON_CRITICAL_ERRORS_ITMP_CHARGER_HAS_OVERFLOW_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_ITMP_CHARGER_HAS_OVERFLOW(
            can_tx_interface));
}

// BMS-17
TEST_F(BmsStateMachineTest, check_cell_monitors_can_signals_in_all_states)
{
    const State *charge_state = App_GetChargeState();

    CheckInRangeCanSignalsInGivenState(
        charge_state, MIN_ITMP_DEGC, MAX_ITMP_DEGC,
        get_segment_0_die_temp_fake.return_val,
        App_CanTx_GetPeriodicSignal_CELL_MONITOR_0_DIE_TEMPERATURE,
        App_CanTx_GetPeriodicSignal_CELL_MONITOR_0_DIE_TEMP_OUT_OF_RANGE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_CELL_MONITOR_0_DIE_TEMP_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_CELL_MONITOR_0_DIE_TEMP_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_CELL_MONITOR_0_DIE_TEMP_OUT_OF_RANGE_OVERFLOW_CHOICE);

    CheckInRangeCanSignalsInGivenState(
        charge_state, MIN_ITMP_DEGC, MAX_ITMP_DEGC,
        get_segment_1_die_temp_fake.return_val,
        App_CanTx_GetPeriodicSignal_CELL_MONITOR_1_DIE_TEMPERATURE,
        App_CanTx_GetPeriodicSignal_CELL_MONITOR_1_DIE_TEMP_OUT_OF_RANGE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_CELL_MONITOR_1_DIE_TEMP_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_CELL_MONITOR_1_DIE_TEMP_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_CELL_MONITOR_1_DIE_TEMP_OUT_OF_RANGE_OVERFLOW_CHOICE);

    CheckInRangeCanSignalsInGivenState(
        charge_state, MIN_ITMP_DEGC, MAX_ITMP_DEGC,
        get_segment_2_die_temp_fake.return_val,
        App_CanTx_GetPeriodicSignal_CELL_MONITOR_2_DIE_TEMPERATURE,
        App_CanTx_GetPeriodicSignal_CELL_MONITOR_2_DIE_TEMP_OUT_OF_RANGE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_CELL_MONITOR_2_DIE_TEMP_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_CELL_MONITOR_2_DIE_TEMP_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_CELL_MONITOR_2_DIE_TEMP_OUT_OF_RANGE_OVERFLOW_CHOICE);

    CheckInRangeCanSignalsInGivenState(
        charge_state, MIN_ITMP_DEGC, MAX_ITMP_DEGC,
        get_segment_3_die_temp_fake.return_val,
        App_CanTx_GetPeriodicSignal_CELL_MONITOR_3_DIE_TEMPERATURE,
        App_CanTx_GetPeriodicSignal_CELL_MONITOR_3_DIE_TEMP_OUT_OF_RANGE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_CELL_MONITOR_3_DIE_TEMP_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_CELL_MONITOR_3_DIE_TEMP_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_CELL_MONITOR_3_DIE_TEMP_OUT_OF_RANGE_OVERFLOW_CHOICE);

    CheckInRangeCanSignalsInGivenState(
        charge_state, MIN_ITMP_DEGC, MAX_ITMP_DEGC,
        get_segment_4_die_temp_fake.return_val,
        App_CanTx_GetPeriodicSignal_CELL_MONITOR_4_DIE_TEMPERATURE,
        App_CanTx_GetPeriodicSignal_CELL_MONITOR_4_DIE_TEMP_OUT_OF_RANGE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_CELL_MONITOR_4_DIE_TEMP_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_CELL_MONITOR_4_DIE_TEMP_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_CELL_MONITOR_4_DIE_TEMP_OUT_OF_RANGE_OVERFLOW_CHOICE);

    CheckInRangeCanSignalsInGivenState(
        charge_state, MIN_ITMP_DEGC, MAX_ITMP_DEGC,
        get_segment_5_die_temp_fake.return_val,
        App_CanTx_GetPeriodicSignal_CELL_MONITOR_5_DIE_TEMPERATURE,
        App_CanTx_GetPeriodicSignal_CELL_MONITOR_5_DIE_TEMP_OUT_OF_RANGE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_CELL_MONITOR_5_DIE_TEMP_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_CELL_MONITOR_5_DIE_TEMP_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_CELL_MONITOR_5_DIE_TEMP_OUT_OF_RANGE_OVERFLOW_CHOICE);
}
// BMS-12
TEST_F(BmsStateMachineTest, check_transition_from_init_state_to_air_open_state)
{
    SetInitialState(App_GetInitState());

    LetTimePass(state_machine, 4999);
    ASSERT_EQ(
        CANMSGS_BMS_STATE_MACHINE_STATE_INIT_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));

    LetTimePass(state_machine, 1);
    ASSERT_EQ(
        CANMSGS_BMS_STATE_MACHINE_STATE_AIR_OPEN_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

// BMS-12
TEST_F(
    BmsStateMachineTest,
    check_transition_from_air_open_state_to_precharge_state)
{
    SetInitialState(App_GetAirOpenState());

    // Check that no state transitions occur. The 500ms duration was chosen
    // arbitrarily.
    LetTimePass(state_machine, 500);
    ASSERT_EQ(
        CANMSGS_BMS_STATE_MACHINE_STATE_AIR_OPEN_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));

    is_air_negative_closed_fake.return_val = true;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(
        CANMSGS_BMS_STATE_MACHINE_STATE_PRE_CHARGE_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

// BMS-41
TEST_F(BmsStateMachineTest, check_for_successful_minimum_duration_precharge)
{
    // Check that state transitions do not occur when the TS voltage is below
    // 93% of the accumulator's maximum voltage while pre-charging capacitors to
    // 90% of the accumulator's max voltage
    SetInitialState(App_GetPreChargeState());
    get_ts_voltage_fake.return_val = std::nextafter(
        MAX_PACK_VOLTAGE * 0.93f, std::numeric_limits<float>::lowest());
    LetTimePass(state_machine, 10);
    ASSERT_EQ(
        CANMSGS_BMS_STATE_MACHINE_STATE_PRE_CHARGE_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));

    LetTimePass(state_machine, MIN_PRE_CHARGE_DURATION_MS - 10);
    ASSERT_EQ(
        CANMSGS_BMS_STATE_MACHINE_STATE_PRE_CHARGE_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

// BMS-41
TEST_F(
    BmsStateMachineTest,
    check_for_voltage_underflow_while_precharging_capacitors_to_90_pct_capacity)
{
    // Check for transition to the init state in the first 10ms when the TS
    // voltage is above 93% of the accumulator's maximum voltage
    SetInitialState(App_GetPreChargeState());
    get_ts_voltage_fake.return_val = std::nextafter(
        MAX_PACK_VOLTAGE * 0.93f, std::numeric_limits<float>::max());
    LetTimePass(state_machine, 10);
    ASSERT_EQ(
        CANMSGS_BMS_STATE_MACHINE_STATE_INIT_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));

    // Check for transition to the init state in the last 10ms when the TS
    // voltage is above 93% of the accumulator's maximum voltage
    SetInitialState(App_GetPreChargeState());
    get_ts_voltage_fake.return_val = MAX_PACK_VOLTAGE * 0.50f;
    LetTimePass(state_machine, MIN_PRE_CHARGE_DURATION_MS - 10);
    ASSERT_EQ(
        CANMSGS_BMS_STATE_MACHINE_STATE_PRE_CHARGE_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));

    get_ts_voltage_fake.return_val = std::nextafter(
        MAX_PACK_VOLTAGE * 0.93f, std::numeric_limits<float>::max());
    LetTimePass(state_machine, 10);
    ASSERT_EQ(
        CANMSGS_BMS_STATE_MACHINE_STATE_INIT_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

// BMS-43
TEST_F(
    BmsStateMachineTest,
    check_can_signals_for_a_pre_charge_undervoltage_error)
{
    SetInitialState(App_GetPreChargeState());

    get_ts_voltage_fake.return_val = MAX_PACK_VOLTAGE * 0.90f;
    LetTimePass(state_machine, MAX_PRE_CHARGE_COMPLETE_DURATION_MS);
    ASSERT_EQ(
        CANMSGS_BMS_STATE_MACHINE_STATE_PRE_CHARGE_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
    ASSERT_EQ(
        CANMSGS_BMS_NON_CRITICAL_ERRORS_PRE_CHARGE_FAULT_OK_CHOICE,
        App_CanTx_GetPeriodicSignal_PRE_CHARGE_FAULT(can_tx_interface));

    get_ts_voltage_fake.return_val = std::nextafter(
        MAX_PACK_VOLTAGE * 0.98f, std::numeric_limits<float>::lowest());
    LetTimePass(state_machine, 10);
    ASSERT_EQ(
        CANMSGS_BMS_STATE_MACHINE_STATE_INIT_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
    ASSERT_EQ(
        CANMSGS_BMS_NON_CRITICAL_ERRORS_PRE_CHARGE_FAULT_UNDERVOLTAGE_CHOICE,
        App_CanTx_GetPeriodicSignal_PRE_CHARGE_FAULT(can_tx_interface));
}

// BMS-43
TEST_F(
    BmsStateMachineTest,
    check_can_signals_for_a_pre_charge_overvoltage_error)
{
    SetInitialState(App_GetPreChargeState());
    get_ts_voltage_fake.return_val = MAX_PACK_VOLTAGE * 0.90f;
    LetTimePass(state_machine, MIN_PRE_CHARGE_COMPLETE_DURATION_MS - 10);
    ASSERT_EQ(
        CANMSGS_BMS_STATE_MACHINE_STATE_PRE_CHARGE_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
    ASSERT_EQ(
        CANMSGS_BMS_NON_CRITICAL_ERRORS_PRE_CHARGE_FAULT_OK_CHOICE,
        App_CanTx_GetPeriodicSignal_PRE_CHARGE_FAULT(can_tx_interface));

    get_ts_voltage_fake.return_val = std::nextafter(
        MAX_PACK_VOLTAGE * 0.98f, std::numeric_limits<float>::max());
    LetTimePass(state_machine, 10);
    ASSERT_EQ(
        CANMSGS_BMS_NON_CRITICAL_ERRORS_PRE_CHARGE_FAULT_OVERVOLTAGE_CHOICE,
        App_CanTx_GetPeriodicSignal_PRE_CHARGE_FAULT(can_tx_interface));
}

TEST_F(
    BmsStateMachineTest,
    check_can_signals_for_a_successful_precharge_at_min_precharge_threshold)
{
    SetInitialState(App_GetPreChargeState());
    get_ts_voltage_fake.return_val = MAX_PACK_VOLTAGE * 0.90f;
    LetTimePass(state_machine, MIN_PRE_CHARGE_COMPLETE_DURATION_MS);

    get_ts_voltage_fake.return_val = MAX_PACK_VOLTAGE * 0.98f;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(
        CANMSGS_BMS_NON_CRITICAL_ERRORS_PRE_CHARGE_FAULT_OK_CHOICE,
        App_CanTx_GetPeriodicSignal_PRE_CHARGE_FAULT(can_tx_interface));
    ASSERT_TRUE(
        (App_CanTx_GetPeriodicSignal_STATE(can_tx_interface) ==
         CANMSGS_BMS_STATE_MACHINE_STATE_CHARGE_CHOICE) ||
        (App_CanTx_GetPeriodicSignal_STATE(can_tx_interface) ==
         CANMSGS_BMS_STATE_MACHINE_STATE_DRIVE_CHOICE));
}

TEST_F(
    BmsStateMachineTest,
    check_can_signals_for_a_successful_precharge_at_max_precharge_threshold)
{
    SetInitialState(App_GetPreChargeState());
    get_ts_voltage_fake.return_val = MAX_PACK_VOLTAGE * 0.90f;
    LetTimePass(state_machine, MAX_PRE_CHARGE_COMPLETE_DURATION_MS - 10);
    ASSERT_EQ(
        CANMSGS_BMS_NON_CRITICAL_ERRORS_PRE_CHARGE_FAULT_OK_CHOICE,
        App_CanTx_GetPeriodicSignal_PRE_CHARGE_FAULT(can_tx_interface));
    ASSERT_EQ(
        CANMSGS_BMS_STATE_MACHINE_STATE_PRE_CHARGE_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));

    get_ts_voltage_fake.return_val = MAX_PACK_VOLTAGE * 0.98f;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(
        CANMSGS_BMS_NON_CRITICAL_ERRORS_PRE_CHARGE_FAULT_OK_CHOICE,
        App_CanTx_GetPeriodicSignal_PRE_CHARGE_FAULT(can_tx_interface));
    ASSERT_TRUE(
        (App_CanTx_GetPeriodicSignal_STATE(can_tx_interface) ==
         CANMSGS_BMS_STATE_MACHINE_STATE_CHARGE_CHOICE) ||
        (App_CanTx_GetPeriodicSignal_STATE(can_tx_interface) ==
         CANMSGS_BMS_STATE_MACHINE_STATE_DRIVE_CHOICE));
}

} // namespace StateMachineTest
