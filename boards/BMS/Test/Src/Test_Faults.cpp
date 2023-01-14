#include "Test_Faults.h"

namespace FaultTest
{
FAKE_VOID_FUNC(send_non_periodic_msg_BMS_STARTUP, const struct CanMsgs_bms_startup_t *);
FAKE_VOID_FUNC(send_non_periodic_msg_BMS_WATCHDOG_TIMEOUT, const struct CanMsgs_bms_watchdog_timeout_t *);
FAKE_VALUE_FUNC(float, get_pwm_frequency);
FAKE_VALUE_FUNC(float, get_pwm_duty_cycle);
FAKE_VALUE_FUNC(uint16_t, get_seconds_since_power_on);
FAKE_VALUE_FUNC(uint32_t, get_current_ms);
FAKE_VOID_FUNC(turn_on_red_led);
FAKE_VOID_FUNC(turn_on_green_led);
FAKE_VOID_FUNC(turn_on_blue_led);
FAKE_VOID_FUNC(enable_charger);
FAKE_VOID_FUNC(disable_charger);
FAKE_VALUE_FUNC(bool, is_charger_connected);
FAKE_VALUE_FUNC(bool, has_charger_faulted);
FAKE_VALUE_FUNC(ExitCode, enable_bms_ok);
FAKE_VALUE_FUNC(ExitCode, disable_bms_ok);
FAKE_VALUE_FUNC(bool, is_bms_ok_enabled);
FAKE_VALUE_FUNC(ExitCode, enable_imd_ok);
FAKE_VALUE_FUNC(ExitCode, disable_imd_ok);
FAKE_VALUE_FUNC(bool, is_imd_ok_enabled);
FAKE_VALUE_FUNC(ExitCode, enable_bspd_ok);
FAKE_VALUE_FUNC(ExitCode, disable_bspd_ok);
FAKE_VALUE_FUNC(bool, is_bspd_ok_enabled);
FAKE_VALUE_FUNC(bool, is_air_negative_closed);
FAKE_VALUE_FUNC(bool, is_air_positive_closed);
FAKE_VOID_FUNC(open_air_positive);
FAKE_VOID_FUNC(close_air_positive);
FAKE_VOID_FUNC(enable_pre_charge);
FAKE_VOID_FUNC(disable_pre_charge);
FAKE_VALUE_FUNC(bool, configure_cell_monitors);
FAKE_VALUE_FUNC(bool, write_cfg_registers);
FAKE_VALUE_FUNC(bool, start_voltage_conv);
FAKE_VALUE_FUNC(bool, read_cell_voltages);
FAKE_VALUE_FUNC(float, get_min_cell_voltage, uint8_t *, uint8_t *);
FAKE_VALUE_FUNC(float, get_max_cell_voltage, uint8_t *, uint8_t *);
FAKE_VALUE_FUNC(float, get_segment_voltage, AccumulatorSegments_E);
FAKE_VALUE_FUNC(float, get_pack_voltage);
FAKE_VALUE_FUNC(float, get_avg_cell_voltage);
FAKE_VALUE_FUNC(float, get_raw_ts_voltage);
FAKE_VALUE_FUNC(float, get_ts_voltage, float);
FAKE_VALUE_FUNC(float, get_raw_low_res_current);
FAKE_VALUE_FUNC(float, get_low_res_current, float);
FAKE_VALUE_FUNC(float, get_raw_high_res_current);
FAKE_VALUE_FUNC(float, get_high_res_current, float);
FAKE_VALUE_FUNC(bool, start_temp_conv);
FAKE_VALUE_FUNC(bool, read_cell_temperatures);
FAKE_VALUE_FUNC(float, get_min_temp_degc, uint8_t *, uint8_t *);
FAKE_VALUE_FUNC(float, get_max_temp_degc, uint8_t *, uint8_t *);
FAKE_VALUE_FUNC(float, get_avg_temp_degc);
FAKE_VALUE_FUNC(bool, enable_discharge);
FAKE_VALUE_FUNC(bool, disable_discharge);

class BmsFaultTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        can_tx_interface =
            App_CanTx_Create(send_non_periodic_msg_BMS_STARTUP, send_non_periodic_msg_BMS_WATCHDOG_TIMEOUT);

        can_rx_interface = App_CanRx_Create();

        imd =
            App_Imd_Create(get_pwm_frequency, IMD_FREQUENCY_TOLERANCE, get_pwm_duty_cycle, get_seconds_since_power_on);

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK);

        rgb_led_sequence = App_SharedRgbLedSequence_Create(turn_on_red_led, turn_on_green_led, turn_on_blue_led);

        charger = App_Charger_Create(enable_charger, disable_charger, is_charger_connected, has_charger_faulted);

        bms_ok = App_OkStatus_Create(enable_bms_ok, disable_bms_ok, is_bms_ok_enabled);

        imd_ok = App_OkStatus_Create(enable_imd_ok, disable_imd_ok, is_imd_ok_enabled);

        bspd_ok = App_OkStatus_Create(enable_bspd_ok, disable_bspd_ok, is_bspd_ok_enabled);

        accumulator = App_Accumulator_Create(
            configure_cell_monitors, write_cfg_registers, start_voltage_conv, read_cell_voltages, get_min_cell_voltage,
            get_max_cell_voltage, get_segment_voltage, get_pack_voltage, get_avg_cell_voltage, start_temp_conv,
            read_cell_temperatures, get_min_temp_degc, get_max_temp_degc, get_avg_temp_degc, enable_discharge,
            disable_discharge);

        precharge_relay = App_PrechargeRelay_Create(enable_pre_charge, disable_pre_charge);

        ts = App_TractiveSystem_Create(
            get_raw_ts_voltage, get_ts_voltage, get_raw_high_res_current, get_high_res_current, get_raw_low_res_current,
            get_low_res_current);

        airs = App_Airs_Create(is_air_positive_closed, is_air_negative_closed, close_air_positive, open_air_positive);

        clock = App_SharedClock_Create();

        world = App_BmsWorld_Create(
            can_tx_interface, can_rx_interface, imd, heartbeat_monitor, rgb_led_sequence, charger, bms_ok, imd_ok,
            bspd_ok, accumulator, airs, precharge_relay, ts, clock);

        // Default to starting the state machine in the `init` state
        state_machine = App_SharedStateMachine_Create(world, App_GetInitState());

        RESET_FAKE(send_non_periodic_msg_BMS_STARTUP);
        RESET_FAKE(send_non_periodic_msg_BMS_WATCHDOG_TIMEOUT);
        RESET_FAKE(get_pwm_frequency);
        RESET_FAKE(get_pwm_duty_cycle);
        RESET_FAKE(get_seconds_since_power_on);
        RESET_FAKE(get_current_ms);
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
        RESET_FAKE(is_air_negative_closed);
        RESET_FAKE(is_air_positive_closed);
        RESET_FAKE(configure_cell_monitors);
        RESET_FAKE(write_cfg_registers);
        RESET_FAKE(start_voltage_conv);
        RESET_FAKE(read_cell_voltages);
        RESET_FAKE(get_min_cell_voltage);
        RESET_FAKE(get_max_cell_voltage);
        RESET_FAKE(get_segment_voltage);
        RESET_FAKE(get_pack_voltage);
        RESET_FAKE(get_avg_cell_voltage);
        RESET_FAKE(get_min_cell_voltage);
        RESET_FAKE(get_max_cell_voltage);
        RESET_FAKE(get_avg_cell_voltage);
        RESET_FAKE(get_min_cell_voltage);
        RESET_FAKE(get_max_cell_voltage);
        RESET_FAKE(get_low_res_current);
        RESET_FAKE(get_raw_low_res_current);
        RESET_FAKE(get_high_res_current);
        RESET_FAKE(get_raw_high_res_current);

        // The charger is connected to prevent other tests from entering the
        // fault state from the charge state
        is_charger_connected_fake.return_val = true;

        // Cell voltages read back are assumed to be true to prevent
        // transitioning into the fault state
        read_cell_voltages_fake.return_val = true;

        // A voltage in [3.0, 4.2] was arbitrarily chosen to prevent other
        // tests from entering the fault state
        get_min_cell_voltage_fake.return_val = 4.0f;
        get_max_cell_voltage_fake.return_val = 4.0f;

        // A temperature in [0.0, 60.0] degC to prevent other tests from entering the fault state
        get_min_temp_degc_fake.return_val = 20.0f;
        get_max_temp_degc_fake.return_val = 20.0f;
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
        TearDownObject(airs, App_Airs_Destroy);
        TearDownObject(precharge_relay, App_PrechargeRelay_Destroy);
        TearDownObject(ts, App_TractiveSystem_Destroy);
        TearDownObject(clock, App_SharedClock_Destroy);
    }

    void SetInitialState(const struct State *const initial_state)
    {
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        state_machine = App_SharedStateMachine_Create(world, initial_state);
        ASSERT_EQ(initial_state, App_SharedStateMachine_GetCurrentState(state_machine));
    }

    std::vector<const struct State *> GetAllStates(void)
    {
        return std::vector<const struct State *>{
            App_GetInitState(), App_GetPreChargeState(), App_GetDriveState(), App_GetChargeState(), App_GetFaultState(),
        };
    }

    void CheckInRangeCanSignalsInGivenState(
        const State *state,
        float        min_value,
        float        max_value,
        float &      fake_value,
        float (*value_can_signal_getter)(const struct BmsCanTxInterface *),
        uint8_t (*out_of_range_can_signal_getter)(const struct BmsCanTxInterface *),
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
        fake_value = std::nextafter(min_value, std::numeric_limits<float>::lowest());
        LetTimePass(state_machine, 1000);
        ASSERT_EQ(underflow_choice, out_of_range_can_signal_getter(can_tx_interface));

        // Overflow range
        fake_value = std::nextafter(max_value, std::numeric_limits<float>::max());
        LetTimePass(state_machine, 1000);
        ASSERT_EQ(overflow_choice, out_of_range_can_signal_getter(can_tx_interface));
    }

    void UpdateClock(struct StateMachine *state_machine, uint32_t current_time_ms) override
    {
        struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
        struct Clock *   clock = App_BmsWorld_GetClock(world);
        App_SharedClock_SetCurrentTimeInMilliseconds(clock, current_time_ms);
    }

    void UpdateSignals(struct StateMachine *state_machine, uint32_t current_time_ms) override
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
    struct Airs *             airs;
    struct PrechargeRelay *   precharge_relay;
    struct TractiveSystem *   ts;
    struct Clock *            clock;
};

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_overvoltage)
{
    // Set TS current positive to trigger discharging condition in voltage check
    get_high_res_current_fake.return_val = 10.0f;
    get_low_res_current_fake.return_val  = 10.0f;

    SetInitialState(App_GetInitState());

    get_max_cell_voltage_fake.return_val = MAX_CELL_VOLTAGE + 1.0f;
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_overtemp_drive_state)
{
    // Set TS current positive to trigger discharging condition in tempertature check
    get_high_res_current_fake.return_val = 10.0f;
    get_low_res_current_fake.return_val  = 10.0f;

    SetInitialState(App_GetInitState());

    // In Discharge state, acceptible temp range is (-20, 60), should be unaffected by temp of 46 C
    get_max_temp_degc_fake.return_val = MAX_CELL_CHARGE_TEMP_DEGC + 1.0f;
    LetTimePass(state_machine, 10);
    ASSERT_NE(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));

    LetTimePass(state_machine, 1000);

    get_max_temp_degc_fake.return_val = MAX_CELL_DISCHARGE_TEMP_DEGC + 1.0f;
    LetTimePass(state_machine, 10);

    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_overtemp_charge_state)
{
    // Set charger conditions such that charger faults do not trigger
    is_charger_connected_fake.return_val   = true;
    is_air_negative_closed_fake.return_val = true;
    has_charger_faulted_fake.return_val    = false;
    get_max_cell_voltage_fake.return_val   = 3.0f;

    // Set TS current negative to trigger charging condition in tempertature check
    get_high_res_current_fake.return_val = -10.0f;
    get_low_res_current_fake.return_val  = -10.0f;

    SetInitialState(App_GetChargeState());

    // In Charge state acceptible temp range is (0, 45)
    get_max_temp_degc_fake.return_val = MAX_CELL_CHARGE_TEMP_DEGC + 1.0f;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_undertemp_drive_state)
{
    // Set TS current positive to trigger discharging condition in tempertature check
    get_high_res_current_fake.return_val = 10.0f;
    get_low_res_current_fake.return_val  = 10.0f;

    SetInitialState(App_GetInitState());

    // In Discharge state, acceptible temp range is (-20, 60), should be unaffected by temp of -1 C
    get_min_temp_degc_fake.return_val = MIN_CELL_CHARGE_TEMP_DEGC - 1.0f;
    LetTimePass(state_machine, 10);
    ASSERT_NE(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));

    LetTimePass(state_machine, 1000);

    get_min_temp_degc_fake.return_val = MIN_CELL_DISCHARGE_TEMP_DEGC - 1.0f;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_undertemp_charge_state)
{
    // Set charger conditions such that charger faults do not trigger
    is_charger_connected_fake.return_val   = true;
    is_air_negative_closed_fake.return_val = true;
    has_charger_faulted_fake.return_val    = false;
    get_max_cell_voltage_fake.return_val   = 3.0f;

    // Set TS current negative to trigger charging condition in tempertature check
    get_high_res_current_fake.return_val = -10.0f;
    get_low_res_current_fake.return_val  = -10.0f;

    SetInitialState(App_GetChargeState());

    // In Charge state acceptable temp range is (0, 45)
    get_min_temp_degc_fake.return_val = MIN_CELL_CHARGE_TEMP_DEGC - 1.0f;
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_ts_discharge_overcurrent)
{
    // Set TS current positive to trigger discharging condition
    get_high_res_current_fake.return_val = 10.0f;
    get_low_res_current_fake.return_val  = 10.0f;

    SetInitialState(App_GetInitState());

    // Max acceptable discharge current is 88.5A*3 = 265.5A
    get_high_res_current_fake.return_val = MAX_TS_DISCHARGE_CURRENT_AMPS + 1.0f;
    get_low_res_current_fake.return_val  = MAX_TS_DISCHARGE_CURRENT_AMPS + 1.0f;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
}
TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_ts_charge_overcurrent)
{
    // Set charger conditions such that charger faults do not trigger
    is_charger_connected_fake.return_val   = true;
    is_air_negative_closed_fake.return_val = true;
    has_charger_faulted_fake.return_val    = false;
    get_max_cell_voltage_fake.return_val   = 3.0f;

    // Set TS current negative to trigger charging condition
    get_high_res_current_fake.return_val = -10.0f;
    get_low_res_current_fake.return_val  = -10.0f;

    SetInitialState(App_GetChargeState());

    // Max acceptable charge current is 23.6A * 3 = 70.8A
    // Charge current is negative
    get_high_res_current_fake.return_val = MAX_TS_CHARGE_CURRENT_AMPS + (-1.0f);
    get_low_res_current_fake.return_val  = MAX_TS_CHARGE_CURRENT_AMPS + (-1.0f);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

TEST_F(BmsFaultTest, check_precharge_fault)
{
    // Possible combinations of is_charger_connected, is_ts_rising_slowly and is_ts_rising_quickly
    bool inputs[8][3] = { { false, false, false }, { false, true, false }, { false, false, true },
                          { false, true, true },   { true, false, false }, { true, true, false },
                          { true, false, true },   { true, true, true } };
    // Possible outputs based on above combinations of inputs
    bool expected_output[8]             = { false, true, true, true, false, true, false, true };
    bool precharge_fault_limit_exceeded = false;

    for (int i = 0; i < 8; i++)
    {
        ASSERT_EQ(
            expected_output[i], App_PrechargeRelay_CheckFaults(
                                    precharge_relay, can_tx_interface, inputs[i][0], inputs[i][1], inputs[i][2],
                                    &precharge_fault_limit_exceeded));

        if (App_PrechargeRelay_GetFaultCounterVal(precharge_relay) >= 3)
        {
            ASSERT_EQ(precharge_fault_limit_exceeded, true);
        }
    }
    // Check to see if counter is non-zero before resetting
    ASSERT_NE(App_PrechargeRelay_GetFaultCounterVal(precharge_relay), 0);

    // counter should be zero after reset
    App_PrechargeRelay_ResetFaultCounterVal(precharge_relay);
    ASSERT_EQ(App_PrechargeRelay_GetFaultCounterVal(precharge_relay), 0);
}
} // namespace FaultTest
