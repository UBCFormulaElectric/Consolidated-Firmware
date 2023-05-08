#include "Test_StateMachine.h"

namespace StateMachineTest
{
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
FAKE_VALUE_FUNC(ExitCode, read_die_temperatures);
FAKE_VALUE_FUNC(bool, is_air_negative_on);
FAKE_VALUE_FUNC(bool, is_air_positive_on);
FAKE_VOID_FUNC(open_air_positive);
FAKE_VOID_FUNC(close_air_positive);
FAKE_VOID_FUNC(enable_pre_charge);
FAKE_VOID_FUNC(disable_pre_charge);
FAKE_VALUE_FUNC(bool, configure_cell_monitors);
FAKE_VALUE_FUNC(bool, write_cfg_registers);
FAKE_VALUE_FUNC(bool, start_voltage_conv);
FAKE_VALUE_FUNC(float, get_min_cell_voltage, uint8_t *, uint8_t *);
FAKE_VALUE_FUNC(float, get_max_cell_voltage, uint8_t *, uint8_t *);
FAKE_VALUE_FUNC(float, get_segment_voltage, AccumulatorSegment);
FAKE_VALUE_FUNC(float, get_pack_voltage);
FAKE_VALUE_FUNC(float, get_avg_cell_voltage);
FAKE_VALUE_FUNC(float, get_ts_voltage);
FAKE_VALUE_FUNC(float, get_low_res_current);
FAKE_VALUE_FUNC(float, get_high_res_current);
FAKE_VALUE_FUNC(bool, start_temp_conv);
FAKE_VALUE_FUNC(bool, read_cell_temperatures);
FAKE_VALUE_FUNC(float, get_min_temp_degc, uint8_t *, uint8_t *);
FAKE_VALUE_FUNC(float, get_max_temp_degc, uint8_t *, uint8_t *);
FAKE_VALUE_FUNC(float, get_avg_temp_degc);
FAKE_VALUE_FUNC(bool, enable_discharge);
FAKE_VALUE_FUNC(bool, disable_discharge);
FAKE_VALUE_FUNC(EEPROM_StatusTypeDef, read_page, uint16_t, uint8_t, uint8_t *, uint16_t);
FAKE_VALUE_FUNC(EEPROM_StatusTypeDef, write_page, uint16_t, uint8_t, uint8_t *, uint16_t);
FAKE_VALUE_FUNC(EEPROM_StatusTypeDef, page_erase, uint16_t);

static float cell_voltages[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT];

static bool read_cell_voltages(float voltages[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT])
{
    for (uint8_t segment = 0; segment < ACCUMULATOR_NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0; cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; cell++)
        {
            voltages[segment][cell] = cell_voltages[segment][cell];
        }
    }

    return true;
}

static void set_cell_voltage(AccumulatorSegment segment, uint8_t cell, float voltage)
{
    if (segment < ACCUMULATOR_NUM_SEGMENTS && cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT)
    {
        cell_voltages[segment][cell] = voltage;
    }
}

static void set_all_cell_voltages(float voltage)
{
    for (uint8_t segment = 0; segment < ACCUMULATOR_NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0; cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; cell++)
        {
            set_cell_voltage((AccumulatorSegment)segment, cell, voltage);
        }
    }
}

class BmsStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        App_CanTx_Init();
        App_CanRx_Init();

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
            configure_cell_monitors, write_cfg_registers, start_voltage_conv, read_cell_voltages, start_temp_conv,
            read_cell_temperatures, get_min_temp_degc, get_max_temp_degc, get_avg_temp_degc, enable_discharge,
            disable_discharge);

        precharge_relay = App_PrechargeRelay_Create(enable_pre_charge, disable_pre_charge);

        ts = App_TractiveSystem_Create(get_ts_voltage, get_high_res_current, get_low_res_current);

        airs = App_Airs_Create(is_air_positive_closed, is_air_negative_closed, close_air_positive, open_air_positive);

        clock = App_SharedClock_Create();

        eeprom = App_Eeprom_Create(write_page, read_page, page_erase);

        world = App_BmsWorld_Create(
            imd, heartbeat_monitor, rgb_led_sequence, charger, bms_ok, imd_ok, bspd_ok, accumulator, airs,
            precharge_relay, ts, clock, eeprom);

        // Default to starting the state machine in the `init` state
        state_machine = App_SharedStateMachine_Create(world, App_GetInitState());
        App_AllStates_Init();

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
        RESET_FAKE(close_air_positive);
        RESET_FAKE(configure_cell_monitors);
        RESET_FAKE(write_cfg_registers);
        RESET_FAKE(start_voltage_conv);
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
        RESET_FAKE(get_high_res_current);
        RESET_FAKE(read_page);
        RESET_FAKE(write_page);
        RESET_FAKE(page_erase);

        // Set initial voltages to nominal value
        set_all_cell_voltages(3.8);
        start_voltage_conv_fake.return_val = true;

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
        TearDownObject(eeprom, App_Eeprom_Destroy);
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
    struct Eeprom *           eeprom;
};

TEST_F(BmsStateMachineTest, check_init_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetInitState());
    EXPECT_EQ(BMS_INIT_STATE, App_CanTx_BMS_Vitals_CurrentState_Get());
}

TEST_F(BmsStateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetDriveState());

    EXPECT_EQ(BMS_DRIVE_STATE, App_CanTx_BMS_Vitals_CurrentState_Get());
}

TEST_F(BmsStateMachineTest, check_fault_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetFaultState());

    EXPECT_EQ(BMS_FAULT_STATE, App_CanTx_BMS_Vitals_CurrentState_Get());
}

TEST_F(BmsStateMachineTest, check_charge_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetChargeState());
    EXPECT_EQ(BMS_CHARGE_STATE, App_CanTx_BMS_Vitals_CurrentState_Get());
}

TEST_F(BmsStateMachineTest, check_imd_frequency_is_broadcasted_over_can_in_all_states)
{
    float fake_frequency = 0.0f;

    for (const auto &state : GetAllStates())
    {
        SetInitialState(state);
        get_pwm_frequency_fake.return_val = fake_frequency;
        LetTimePass(state_machine, 10);

        EXPECT_EQ(fake_frequency, App_CanTx_BMS_ImdPwmOutput_Frequency_Get());

        // To avoid false positives, we use a different duty cycle each time
        fake_frequency++;
    }
}

TEST_F(BmsStateMachineTest, check_imd_duty_cycle_is_broadcasted_over_can_in_all_states)
{
    float fake_duty_cycle = 0.0f;

    for (const auto &state : GetAllStates())
    {
        SetInitialState(state);
        get_pwm_duty_cycle_fake.return_val = fake_duty_cycle;
        LetTimePass(state_machine, 10);

        EXPECT_EQ(fake_duty_cycle, App_CanTx_BMS_ImdPwmOutput_DutyCycle_Get());

        // To avoid false positives, we use a different frequency each time
        fake_duty_cycle++;
    }
}

TEST_F(BmsStateMachineTest, check_imd_insulation_resistance_10hz_is_broadcasted_over_can_in_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);
        ImdTest::SetImdCondition(imd, IMD_NORMAL, get_pwm_frequency_fake.return_val);

        // Test an arbitrarily chosen valid resistance
        get_pwm_duty_cycle_fake.return_val = 50.0f;
        LetTimePass(state_machine, 10);

        EXPECT_EQ(IMD_NORMAL, App_CanTx_BMS_ImdStatus_Condition_Get());
        EXPECT_EQ(true, App_CanTx_BMS_ImdStatus_ValidDutyCycle_Get());
        EXPECT_EQ(1200, App_CanTx_BMS_ImdData_InsulationMeasurementDcp10Hz_Get());

        // Test an arbitrarily chosen invalid resistance
        get_pwm_duty_cycle_fake.return_val = 0.0f;
        LetTimePass(state_machine, 10);
        EXPECT_EQ(IMD_NORMAL, App_CanTx_BMS_ImdStatus_Condition_Get());
        EXPECT_EQ(false, App_CanTx_BMS_ImdStatus_ValidDutyCycle_Get());
    }
}

TEST_F(BmsStateMachineTest, check_imd_insulation_resistance_20hz_is_broadcasted_over_can_in_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);
        ImdTest::SetImdCondition(imd, IMD_UNDERVOLTAGE_DETECTED, get_pwm_frequency_fake.return_val);

        // Test an arbitrarily chosen valid resistance
        get_pwm_duty_cycle_fake.return_val = 50.0f;
        LetTimePass(state_machine, 10);

        EXPECT_EQ(IMD_UNDERVOLTAGE_DETECTED, App_CanTx_BMS_ImdStatus_Condition_Get());
        EXPECT_EQ(true, App_CanTx_BMS_ImdStatus_ValidDutyCycle_Get());
        EXPECT_EQ(1200, App_CanTx_BMS_ImdData_InsulationMeasurementDcp20Hz_Get());

        // Test an arbitrarily chosen invalid resistance
        get_pwm_duty_cycle_fake.return_val = 0.0f;
        LetTimePass(state_machine, 10);
        EXPECT_EQ(IMD_UNDERVOLTAGE_DETECTED, App_CanTx_BMS_ImdStatus_Condition_Get());
        EXPECT_EQ(false, App_CanTx_BMS_ImdStatus_ValidDutyCycle_Get());
    }
}

TEST_F(BmsStateMachineTest, check_imd_speed_start_status_30hz_is_broadcasted_over_can_in_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);
        ImdTest::SetImdCondition(imd, IMD_SST, get_pwm_frequency_fake.return_val);

        // Test an arbitrarily chosen SST_GOOD
        get_pwm_duty_cycle_fake.return_val = 7.5f;
        LetTimePass(state_machine, 10);
        EXPECT_EQ(IMD_SST, App_CanTx_BMS_ImdStatus_Condition_Get());
        EXPECT_EQ(true, App_CanTx_BMS_ImdStatus_ValidDutyCycle_Get());
        EXPECT_EQ(SST_GOOD, App_CanTx_BMS_ImdData_SpeedStartStatus30Hz_Get());

        // Test an arbitrarily chosen SST_BAD
        get_pwm_duty_cycle_fake.return_val = 92.5f;
        LetTimePass(state_machine, 10);
        EXPECT_EQ(IMD_SST, App_CanTx_BMS_ImdStatus_Condition_Get());
        EXPECT_EQ(true, App_CanTx_BMS_ImdStatus_ValidDutyCycle_Get());
        EXPECT_EQ(SST_BAD, App_CanTx_BMS_ImdData_SpeedStartStatus30Hz_Get());

        // Test an arbitrarily chosen invalid SST status
        get_pwm_duty_cycle_fake.return_val = 0.0f;
        LetTimePass(state_machine, 10);
        EXPECT_EQ(IMD_SST, App_CanTx_BMS_ImdStatus_Condition_Get());
        EXPECT_EQ(false, App_CanTx_BMS_ImdStatus_ValidDutyCycle_Get());
    }
}

TEST_F(BmsStateMachineTest, check_imd_seconds_since_power_on_is_broadcasted_over_can_in_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);
        get_seconds_since_power_on_fake.return_val = 123;
        LetTimePass(state_machine, 10);
        EXPECT_EQ(123, App_CanTx_BMS_ImdStatus_SecondsSincePowerOn_Get());
    }
}

TEST_F(BmsStateMachineTest, rgb_led_sequence_in_all_states)
{
    unsigned int *call_counts[] = { &turn_on_red_led_fake.call_count, &turn_on_green_led_fake.call_count,
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

TEST_F(BmsStateMachineTest, charger_connection_status_in_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        is_charger_connected_fake.return_val = true;
        LetTimePass(state_machine, 1000);
        EXPECT_EQ(true, App_CanTx_BMS_Charger_IsConnected_Get());

        is_charger_connected_fake.return_val = false;
        LetTimePass(state_machine, 1000);
        EXPECT_EQ(false, App_CanTx_BMS_Charger_IsConnected_Get());
    }
}

TEST_F(BmsStateMachineTest, check_bms_ok_is_broadcasted_over_can_in_all_states)
{
    // Enable BMS_OK
    is_bms_ok_enabled_fake.return_val = true;

    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        // Make sure the state machine sets the CAN signal for BMS_OK
        LetTimePass(state_machine, 10);
        ASSERT_EQ(true, App_CanTx_BMS_OkStatuses_BmsOk_Get());

        // Reset the CAN signal for BMS_OK
        App_CanTx_BMS_OkStatuses_BmsOk_Set(false);
    }
}

TEST_F(BmsStateMachineTest, check_imd_ok_is_broadcasted_over_can_in_all_states)
{
    // Enable IMD_OK
    is_imd_ok_enabled_fake.return_val = true;

    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        // Make sure the state machine sets the CAN signal for IMD_OK
        LetTimePass(state_machine, 10);
        ASSERT_EQ(true, App_CanTx_BMS_OkStatuses_ImdOk_Get());

        // Reset the CAN signal for IMD_OK
        App_CanTx_BMS_OkStatuses_ImdOk_Set(false);
    }
}

TEST_F(BmsStateMachineTest, check_bspd_ok_is_broadcasted_over_can_in_all_states)
{
    // Enable BSPD_OK
    is_bspd_ok_enabled_fake.return_val = true;

    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        // Make sure the state machine sets the CAN signal for BSPD_OK
        LetTimePass(state_machine, 10);
        ASSERT_EQ(true, App_CanTx_BMS_OkStatuses_BspdOk_Get());

        // Reset the CAN signal for BSPD_OK
        App_CanTx_BMS_OkStatuses_BspdOk_Set(false);
    }
}

TEST_F(BmsStateMachineTest, charger_disconnects_in_charge_state)
{
    SetInitialState(App_GetChargeState());

    is_air_negative_closed_fake.return_val = true;

    // Set the current values to above the threshold for charging to stop (charging should continue)
    get_high_res_current_fake.return_val = 1.0f;
    get_low_res_current_fake.return_val  = 1.0f;

    // Simulate situation with charger present and user indicate to start charging
    is_charger_connected_fake.return_val = false;
    App_CanRx_DEBUG_ChargingSwitch_StartCharging_Update(true);

    LetTimePass(state_machine, 10);

    // Checks if a CAN message was sent to indicate charger was disconnected unexpectedly
    ASSERT_EQ(true, App_CanAlerts_GetFault(BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE));
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

TEST_F(BmsStateMachineTest, check_airs_can_signals_for_all_states)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        is_air_negative_closed_fake.return_val = false;
        is_air_positive_closed_fake.return_val = false;
        LetTimePass(state_machine, 10);
        ASSERT_EQ(false, App_CanTx_BMS_Contactors_AirNegative_Get());
        ASSERT_EQ(false, App_CanTx_BMS_Contactors_AirPositive_Get());

        is_air_negative_closed_fake.return_val = false;
        is_air_positive_closed_fake.return_val = true;
        LetTimePass(state_machine, 10);
        ASSERT_EQ(false, App_CanTx_BMS_Contactors_AirNegative_Get());
        ASSERT_EQ(true, App_CanTx_BMS_Contactors_AirPositive_Get());

        is_air_negative_closed_fake.return_val = true;
        is_air_positive_closed_fake.return_val = false;
        LetTimePass(state_machine, 10);
        ASSERT_EQ(true, App_CanTx_BMS_Contactors_AirNegative_Get());
        ASSERT_EQ(false, App_CanTx_BMS_Contactors_AirPositive_Get());

        is_air_negative_closed_fake.return_val = true;
        is_air_positive_closed_fake.return_val = true;
        LetTimePass(state_machine, 10);
        ASSERT_EQ(true, App_CanTx_BMS_Contactors_AirNegative_Get());
        ASSERT_EQ(true, App_CanTx_BMS_Contactors_AirPositive_Get());
    }
}

TEST_F(BmsStateMachineTest, check_contactors_open_in_fault_state)
{
    // Close AIR+ to avoid false positives
    App_CanTx_BMS_Contactors_AirPositive_Set(CONTACTOR_STATE_CLOSED);

    SetInitialState(App_GetFaultState());
    ASSERT_EQ(CONTACTOR_STATE_OPEN, App_CanTx_BMS_Contactors_AirPositive_Get());
}

TEST_F(BmsStateMachineTest, check_state_transition_from_fault_to_init_with_no_faults_set)
{
    // Assume no AIR shutdown faults have been set
    SetInitialState(App_GetFaultState());

    // If charger is connected, having air_negative open will lead to fault state, so ensure it is not
    is_charger_connected_fake.return_val   = false;
    is_air_negative_closed_fake.return_val = true;
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(BMS_FAULT_STATE, App_CanTx_BMS_Vitals_CurrentState_Get());

    is_air_negative_closed_fake.return_val = false;
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(BMS_INIT_STATE, App_CanTx_BMS_Vitals_CurrentState_Get());
}

TEST_F(BmsStateMachineTest, check_state_transition_from_fault_to_init_with_air_negative_open)
{
    SetInitialState(App_GetFaultState());

    // Check that state machine remains in FaultState with AIR- closed
    // If charger is connected, having air_negative open will lead to fault state, so ensure it is not
    is_air_negative_closed_fake.return_val = true;
    is_charger_connected_fake.return_val   = false;
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(BMS_FAULT_STATE, App_CanTx_BMS_Vitals_CurrentState_Get());

    // Check that state mcachine transitions to InitState with AIR- open
    is_air_negative_closed_fake.return_val = false;
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(BMS_INIT_STATE, App_CanTx_BMS_Vitals_CurrentState_Get());
}

TEST_F(BmsStateMachineTest, charger_connected_no_can_msg_init_state)
{
    SetInitialState(App_GetInitState());

    is_air_negative_closed_fake.return_val = true;

    // Without the CAN message to start charging, will remain in init state when charger is connected
    is_charger_connected_fake.return_val = true;

    LetTimePass(state_machine, 20);

    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

TEST_F(BmsStateMachineTest, charger_connected_can_msg_init_state)
{
    SetInitialState(App_GetInitState());

    is_air_negative_closed_fake.return_val = true;

    // Simulate situation with charger present and user indicate to start charging
    is_charger_connected_fake.return_val = true;

    App_CanRx_DEBUG_ChargingSwitch_StartCharging_Update(true);

    LetTimePass(state_machine, 10);

    ASSERT_EQ(App_GetPreChargeState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

TEST_F(BmsStateMachineTest, charger_connected_successful_precharge_stays)
{
    SetInitialState(App_GetInitState());

    is_air_negative_closed_fake.return_val = true;

    // Set the current values to above the threshold for charging to stop (charging should continue)
    get_high_res_current_fake.return_val = 1.0f;
    get_low_res_current_fake.return_val  = 1.0f;

    // Simulate situation with charger present and user indicate to start charging
    is_charger_connected_fake.return_val = true;
    App_CanRx_DEBUG_ChargingSwitch_StartCharging_Update(true);

    // Allow BMS time to go through Init state
    LetTimePass(state_machine, 20);
    get_ts_voltage_fake.return_val = 400;

    // Pause for slightly longer to allow pre-charge
    LetTimePass(state_machine, 50);

    printf("%s", App_SharedStateMachine_GetCurrentState(state_machine)->name);
    ASSERT_EQ(App_GetChargeState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

TEST_F(BmsStateMachineTest, keeps_charging_with_no_interrupts)
{
    SetInitialState(App_GetChargeState());

    is_air_negative_closed_fake.return_val = true;

    get_high_res_current_fake.return_val = 1.0f;
    get_low_res_current_fake.return_val  = 1.0f;

    // Simulate situation with charger present and user indicate to start charging
    is_charger_connected_fake.return_val = true;

    App_CanRx_DEBUG_ChargingSwitch_StartCharging_Update(true);

    LetTimePass(state_machine, 100);

    ASSERT_EQ(true, App_Charger_IsEnabled(charger));
    ASSERT_EQ(App_GetChargeState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

TEST_F(BmsStateMachineTest, stops_charging_at_full_charge)
{
    SetInitialState(App_GetChargeState());

    is_air_negative_closed_fake.return_val = true;

    // Setting current below threshold for full charge (charging should stop)
    get_high_res_current_fake.return_val = 0.8f;
    get_low_res_current_fake.return_val  = 0.8f;

    // Simulate situation with charger present and user indicate to start charging
    is_charger_connected_fake.return_val = true;

    App_CanRx_DEBUG_ChargingSwitch_StartCharging_Update(true);

    LetTimePass(state_machine, 10);

    ASSERT_EQ(false, App_Charger_IsEnabled(charger));
    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

TEST_F(BmsStateMachineTest, stops_charging_after_false_charging_msg)
{
    SetInitialState(App_GetChargeState());

    is_air_negative_closed_fake.return_val = true;

    // Set the current values to above the threshold for charging to stop (charging should continue)
    get_high_res_current_fake.return_val = 1.0f;
    get_low_res_current_fake.return_val  = 1.0f;

    // Simulate situation with charger present and user indicate to start charging
    is_charger_connected_fake.return_val = true;

    App_CanRx_DEBUG_ChargingSwitch_StartCharging_Update(false);

    LetTimePass(state_machine, 10);

    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

TEST_F(BmsStateMachineTest, fault_from_charger_fault)
{
    SetInitialState(App_GetChargeState());

    is_air_negative_closed_fake.return_val = true;

    // Set the current values to above the threshold for charging to stop (charging should continue)
    get_high_res_current_fake.return_val = 1.0f;
    get_low_res_current_fake.return_val  = 1.0f;

    // Simulate situation with charger present and user indicate to start charging
    is_charger_connected_fake.return_val = true;

    has_charger_faulted_fake.return_val = true;

    App_Charger_GetCounterVal_fake.return_val = 500U;

    App_CanRx_DEBUG_ChargingSwitch_StartCharging_Update(true);

    // Charger faults are ignored for 5s upon charge state entry
    LetTimePass(state_machine, 5010);
    const struct State *currentState = App_SharedStateMachine_GetCurrentState(state_machine);

    ASSERT_EQ(App_GetFaultState(), currentState);
}

TEST_F(BmsStateMachineTest, faults_after_shutdown_loop_activates_while_charging)
{
    SetInitialState(App_GetChargeState());

    is_air_negative_closed_fake.return_val = true;

    // Set the current values to above the threshold for charging to stop (charging should continue)
    get_high_res_current_fake.return_val = 1.0f;
    get_low_res_current_fake.return_val  = 1.0f;

    App_CanRx_DEBUG_ChargingSwitch_StartCharging_Update(true);

    // Letting time pass starts the charging process
    LetTimePass(state_machine, 10);

    // Set current to high value for a hard-coded fault that remains uncleared
    get_high_res_current_fake.return_val   = 1000.0f;
    get_low_res_current_fake.return_val    = 1000.0f;
    is_air_negative_closed_fake.return_val = false;

    // Simulate situation with charger present and user indicate to start charging
    is_charger_connected_fake.return_val = true;

    LetTimePass(state_machine, 20);

    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

TEST_F(BmsStateMachineTest, check_remains_in_fault_state_until_fault_cleared_then_transitions_to_init)
{
    SetInitialState(App_GetInitState());

    // Let accumulator startup count expire
    LetTimePass(state_machine, 1000);

    // Set TS current positive to trigger discharging condition in tempertature check
    get_high_res_current_fake.return_val = 10.0f;
    get_low_res_current_fake.return_val  = 10.0f;

    // Simulate over-temp fault in drive state
    get_max_temp_degc_fake.return_val = MAX_CELL_DISCHARGE_TEMP_DEGC + 1.0f;
    LetTimePass(state_machine, 10);

    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));

    // Check that state machine remains in fault state without cycling to init state for long period of time
    for (int i = 0; i < 1000; i++)
    {
        LetTimePass(state_machine, 1);
        ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
    }

    // Remove fault condition and check transition to init state
    get_max_temp_degc_fake.return_val = MAX_CELL_DISCHARGE_TEMP_DEGC - 10.0f;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

TEST_F(BmsStateMachineTest, check_precharge_state_transitions_and_air_plus_status)
{
    struct
    {
        bool  air_negative_closes;
        float initial_ts_voltage;
        float precharge_duration;
        bool  expect_precharge_starts;
        bool  expect_precharge_successful;
    } test_params[5] = { {
                             // Precharge doesn't start, AIR- doesn't close
                             .air_negative_closes         = false,
                             .initial_ts_voltage          = 0.0,
                             .precharge_duration          = PRECHARGE_COMPLETION_MS,
                             .expect_precharge_starts     = false,
                             .expect_precharge_successful = false,
                         },
                         {
                             // Precharge doesn't start, TS voltage too high
                             .air_negative_closes         = false,
                             .initial_ts_voltage          = 11.0, // 10V is threshold to precharge
                             .precharge_duration          = PRECHARGE_COMPLETION_MS,
                             .expect_precharge_starts     = false,
                             .expect_precharge_successful = false,
                         },
                         {
                             // Nominal precharge, success
                             .air_negative_closes         = true,
                             .initial_ts_voltage          = 0.0,
                             .precharge_duration          = PRECHARGE_COMPLETION_MS,
                             .expect_precharge_starts     = true,
                             .expect_precharge_successful = true,
                         },
                         {
                             // Fast precharge, fails
                             .air_negative_closes         = true,
                             .initial_ts_voltage          = 0.0,
                             .precharge_duration          = PRECHARGE_COMPLETION_LOWER_BOUND - 30,
                             .expect_precharge_starts     = true,
                             .expect_precharge_successful = false,
                         },
                         {
                             // Slow precharge, fails
                             .air_negative_closes         = true,
                             .initial_ts_voltage          = 0.0,
                             .precharge_duration          = PRECHARGE_COMPLETION_UPPER_BOUND + 30,
                             .expect_precharge_starts     = true,
                             .expect_precharge_successful = false,
                         } };

    for (int i = 0; i < 5; i++)
    {
        TearDown();
        SetUp();

        // Let accumulator startup count expire
        LetTimePass(state_machine, 1000);

        // Set initial conditions for precharge
        is_air_negative_closed_fake.return_val = test_params[i].air_negative_closes;
        get_ts_voltage_fake.return_val         = test_params[i].initial_ts_voltage;

        if (test_params[i].expect_precharge_starts)
        {
            // Precharge should start
            LetTimePass(state_machine, 10);
            ASSERT_EQ(App_GetPreChargeState(), App_SharedStateMachine_GetCurrentState(state_machine));
            ASSERT_EQ(close_air_positive_fake.call_count, 0);

            // Let precharge duration elapse, confirm still in precharge state and AIR+ open
            LetTimePass(state_machine, test_params[i].precharge_duration - 10);
            ASSERT_EQ(App_GetPreChargeState(), App_SharedStateMachine_GetCurrentState(state_machine));
            ASSERT_EQ(close_air_positive_fake.call_count, 0);

            // Set voltage to pack voltage (i.e. voltage successfully rose within duration)
            get_ts_voltage_fake.return_val = 3.8f * ACCUMULATOR_NUM_SEGMENTS * ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT;
            LetTimePass(state_machine, 10);

            if (test_params[i].expect_precharge_successful)
            {
                // Precharge successful, enter drive
                ASSERT_EQ(App_GetDriveState(), App_SharedStateMachine_GetCurrentState(state_machine));
                ASSERT_EQ(close_air_positive_fake.call_count, 1);

                LetTimePass(state_machine, 1000);
                ASSERT_EQ(App_GetDriveState(), App_SharedStateMachine_GetCurrentState(state_machine));
                ASSERT_EQ(close_air_positive_fake.call_count, 1);
            }
            else
            {
                // Precharge failed, back to init to try again
                ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
                ASSERT_EQ(close_air_positive_fake.call_count, 0);

                LetTimePass(state_machine, 1000);
                ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
                ASSERT_EQ(close_air_positive_fake.call_count, 0);
            }
        }
        else
        {
            // Precharge doesn't start, stay in init indefinitely
            ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
            ASSERT_EQ(close_air_positive_fake.call_count, 0);

            LetTimePass(state_machine, 1000);
            ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
            ASSERT_EQ(close_air_positive_fake.call_count, 0);
        }
    }
}


} // namespace StateMachineTest
